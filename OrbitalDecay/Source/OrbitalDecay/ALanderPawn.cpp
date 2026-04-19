#include "ALanderPawn.h"
#include "Camera/CameraComponent.h"
#include "OrbitalDecayGameMode.h"

ALanderPawn::ALanderPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootTransform"));
    RootComponent = Root;

    // Now assigned to member variable, not a local
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanderBody"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetMobility(EComponentMobility::Movable);

    // Now assigned to member variable, not a local
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("LanderCamera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
}

void ALanderPawn::BeginPlay()
{
    Super::BeginPlay();
    TargetRotation = GetActorRotation();
}

void ALanderPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

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
        CurrentAltitude = HitResult.Distance * 0.0328084f;
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