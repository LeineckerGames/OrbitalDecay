#include "ALanderPawn.h"
#include "Camera/CameraComponent.h"
#include "OrbitalDecayGameMode.h"

ALanderPawn::ALanderPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanderBody"));
    RootComponent = Mesh;
    Mesh->SetMobility(EComponentMobility::Movable);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("LanderCamera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
    Camera->SetRelativeRotation(CameraRotation);
}

void ALanderPawn::BeginPlay()
{
    Super::BeginPlay();
    TargetRotation = GetActorRotation();
}

void ALanderPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bHasLanded) return;

    AOrbitalDecayGameMode* MyGameMode = Cast<AOrbitalDecayGameMode>(GetWorld()->GetAuthGameMode());
    if (MyGameMode)
    {
        int32 CurrentLevel = MyGameMode->GlobalLevel;
        int32 SteppedLevel = (CurrentLevel / 5) * 5;
        float Alpha = FMath::Clamp((float)(SteppedLevel - 1) / 19.0f, 0.0f, 1.0f);
        float GravityStrength = FMath::Lerp(LowGravity, HighGravity, Alpha);

        // Handle boost timer
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
            // Vertical thrust is always applied regardless of mode
            CurrentVelocity.Z += (GravityStrength + DefaultThrust) * DeltaTime;

            if (bForwardThrustMode)
            {
                FVector ForwardDir = GetActorForwardVector();
                float ForwardThrust = bIsBoosting ? BoostedForwardThrust : DefaultForwardThrust;
                CurrentVelocity += ForwardDir * ForwardThrust * DeltaTime;
            }
            else
            {
                // Vertical boost mode - extra upward thrust on top of default
                if (bIsBoosting)
                {
                    CurrentVelocity.Z += BoostedThrust * DeltaTime;
                }
            }

            // Drain fuel
            float DrainRate = bIsBoosting ? FuelDrainBoost : FuelDrainIdle;
            Fuel -= DrainRate * DeltaTime;
            Fuel = FMath::Max(Fuel, 0.0f);
        }
        else
        {
            // No fuel - full gravity, no thrust at all
            CurrentVelocity.Z += GravityStrength * DeltaTime;
        }

        AddActorWorldOffset(CurrentVelocity * DeltaTime, true);
    }

    // Line trace for altitude
    FVector Start = GetActorLocation();
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
            float ImpactVelocity = CurrentVelocity.Z;
            CurrentVelocity = FVector::ZeroVector;
            bIsBoosting = false;

            // Lock position at landing point
            FVector LandedPosition = GetActorLocation();
            LandedPosition.Z = HitResult.ImpactPoint.Z + 50.0f;
            SetActorLocation(LandedPosition);

            if (ImpactVelocity <= MaxSafeLandingVelocity)
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CRASHED!"));
                }
            }
            else
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LANDED SUCCESSFULLY!"));
                }
            }
        }
    }
    else
    {
        CurrentAltitude = 99999.f;
    }

    // Smoothly interpolate rotation towards target
    FRotator CurrentRotation = GetActorRotation();
    FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
    SetActorRotation(SmoothedRotation);
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
            CurrentVelocity += ForwardDir * 200.0f; // Boost in actual facing direction
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