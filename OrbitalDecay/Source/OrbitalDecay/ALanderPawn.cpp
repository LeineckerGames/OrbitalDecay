#include "ALanderPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "OrbitalDecayGameMode.h"
#include "Kismet/GameplayStatics.h"

ALanderPawn::ALanderPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanderBody"));
    RootComponent = Mesh;
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetVisibility(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("LanderCamera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
    Camera->SetRelativeRotation(CameraRotation);

    ReplaySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ReplaySpringArm"));
    ReplaySpringArm->SetupAttachment(RootComponent);
    ReplaySpringArm->TargetArmLength = 1200.f;
    ReplaySpringArm->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
    ReplaySpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
    ReplaySpringArm->bUsePawnControlRotation = false;
    ReplaySpringArm->bDoCollisionTest = false;
    ReplaySpringArm->bInheritPitch = false;
    ReplaySpringArm->bInheritYaw = false;
    ReplaySpringArm->bInheritRoll = false;

    ReplayCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ReplayCamera"));
    ReplayCamera->SetupAttachment(ReplaySpringArm);
    ReplayCamera->SetActive(false);
}

void ALanderPawn::BeginPlay()
{
    Super::BeginPlay();
    TargetRotation = GetActorRotation();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReplayRecorder::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        ReplayRecorder = Cast<AReplayRecorder>(FoundActors[0]);
    }

    if (GEngine)
    {
        if (ReplayRecorder)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ReplayRecorder FOUND"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ReplayRecorder NOT FOUND - is it placed in the level?"));
        }
    }
}

void ALanderPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bool bIsReplaying = ReplayRecorder && ReplayRecorder->IsReplaying();

    if (bLevelComplete && !bIsReplaying) return;

    if (!bIsReplaying)
    {
        if (!bHasLanded)
        {
            AOrbitalDecayGameMode* MyGameMode = Cast<AOrbitalDecayGameMode>(GetWorld()->GetAuthGameMode());
            if (MyGameMode)
            {
                int32 CurrentLevel = MyGameMode->GlobalLevel;
                int32 SteppedLevel = (CurrentLevel / 5) * 5;
                float Alpha = FMath::Clamp((float)(SteppedLevel - 1) / 19.0f, 0.0f, 1.0f);
                float GravityStrength = FMath::Lerp(LowGravity, HighGravity, Alpha);

                if (bIsBoosting)
                {
                    CurrentBoostTimer -= DeltaTime;
                    if (CurrentBoostTimer <= 0.0f)
                    {
                        bIsBoosting = false;
                        CurrentBoostTimer = 0.0f;
                    }
                }

                if (Fuel > 0.0f)
                {
                    if (bForwardThrustMode)
                    {
                        float HoverThrust = -GravityStrength * 0.95f;
                        CurrentVelocity.Z += (GravityStrength + HoverThrust) * DeltaTime;

                        if (bIsBoosting)
                        {
                            FVector ForwardDir = GetActorForwardVector();
                            CurrentVelocity += ForwardDir * BoostedForwardThrust * DeltaTime;
                        }

                        CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X, 0.0f, DeltaTime, AirResistance);
                        CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y, 0.0f, DeltaTime, AirResistance);
                        CurrentVelocity.Z = FMath::Clamp(CurrentVelocity.Z, -50.0f, 0.0f);
                    }
                    else
                    {
                        CurrentVelocity.Z += (GravityStrength + DefaultThrust) * DeltaTime;

                        if (bIsBoosting)
                        {
                            CurrentVelocity.Z += BoostedThrust * DeltaTime;
                        }
                    }

                    float DrainRate = bIsBoosting ? FuelDrainBoost : FuelDrainIdle;
                    Fuel -= DrainRate * DeltaTime;
                    Fuel = FMath::Max(Fuel, 0.0f);
                }
                else
                {
                    // No fuel, only gravity applies
                    CurrentVelocity.Z += GravityStrength * DeltaTime;
                }

                AddActorWorldOffset(CurrentVelocity * DeltaTime, true);
            }
        }

        FVector Start = Camera->GetComponentLocation();
        FVector End = Start + FVector(0.f, 0.f, -100000.f);
        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
        {
            float DistanceCM = HitResult.Distance;
            float DistanceFeet = DistanceCM * 0.0328084f;
            CurrentAltitude = DistanceFeet;

            if (HitResult.Distance < 50.0f && !bHasLanded)
            {
                bHasLanded = true;
                CurrentVelocity = FVector::ZeroVector;
                bIsBoosting = false;

                FVector LandedPosition = GetActorLocation();
                LandedPosition.Z = HitResult.ImpactPoint.Z + 50.0f;
                SetActorLocation(LandedPosition);

                bool bOnLandingPad = false;
                if (HitResult.GetActor())
                {
                    bOnLandingPad = HitResult.GetActor()->ActorHasTag(FName("LandingPad"));
                }

                if (bOnLandingPad)
                {
                    AActor* PadActor = HitResult.GetActor();

                    if (!VisitedPads.Contains(PadActor))
                    {
                        VisitedPads.Add(PadActor);
                        PadsLanded++;

                        if (GEngine)
                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                                FString::Printf(TEXT("LANDED SUCCESSFULLY! (%d/3)"), PadsLanded));

                        if (PadsLanded >= 3)
                        {
                            bLevelComplete = true;
                        }
                    }
                    else
                    {
                        if (GEngine)
                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Already visited this pad!"));
                    }

                    FTimerHandle RelaunchTimer;
                    GetWorldTimerManager().SetTimer(RelaunchTimer, [this]()
                        {
                            bHasLanded = false;
                            CurrentVelocity.Z = 300.0f;
                        }, 1.5f, false);
                }
                else
                {
                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CRASHED! Missed the landing pad!"));

                    FTimerHandle ReplayTimerHandle;
                    GetWorldTimerManager().SetTimer(ReplayTimerHandle, [this]()
                        {
                            StartCrashReplay();
                        }, 1.5f, false);
                }
            }
        }
        else
        {
            CurrentAltitude = 99999.f;
        }

        FRotator CurrentRotation = GetActorRotation();
        FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
        SetActorRotation(SmoothedRotation);
    }
}

void ALanderPawn::StartCrashReplay()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("REPLAY STARTING"));
    }

    if (!ReplayRecorder)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("REPLAYRECORDER IS NULL - NOT ASSIGNED"));
        }
        return;
    }

    Mesh->SetVisibility(true);

    if (Camera) Camera->SetActive(false);
    if (ReplayCamera)
    {
        ReplayCamera->SetActive(true);

        APlayerController* PC = Cast<APlayerController>(GetController());
        if (PC)
        {
            PC->SetViewTargetWithBlend(this, 0.5f);
        }
    }

    ReplayRecorder->StartReplay();
}

void ALanderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALanderPawn::ActivateBoost()
{
    if (Fuel > 0.0f)
    {
        bIsBoosting = true;
        CurrentBoostTimer = BoostDuration;

        if (bForwardThrustMode)
        {
            FVector ForwardDir = GetActorForwardVector();
            CurrentVelocity += ForwardDir * 200.0f;
        }
        else
        {
            CurrentVelocity.Z += 200.0f;
        }
    }
}

void ALanderPawn::AddFuel(float Amount)
{
    Fuel = FMath::Clamp(Fuel + Amount, 0.0f, MaxFuel);
}

void ALanderPawn::ToggleThrustMode()
{
    bForwardThrustMode = !bForwardThrustMode;
}

void ALanderPawn::RotateLeft()
{
    TargetRotation.Yaw -= RotationStep;
}

void ALanderPawn::RotateRight()
{
    TargetRotation.Yaw += RotationStep;
}
