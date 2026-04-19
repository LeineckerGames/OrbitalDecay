#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "ALanderPawn.generated.h"

UCLASS()
class ORBITALDECAY_API ALanderPawn : public APawn
{
    GENERATED_BODY()

public:
    ALanderPawn();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, Category = "Flight")
    int32 MyCurrentLevel = 1;

    UPROPERTY(EditAnywhere, Category = "Flight")
    FVector CurrentVelocity = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float CurrentAltitude = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float Fuel = 1000.0f; // Starting fuel, player can change this later

    UPROPERTY(EditAnywhere, Category = "Flight")
    float MaxFuel = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float DefaultThrust = 100.0f; // Slows fall but doesn't stop it

    UPROPERTY(EditAnywhere, Category = "Flight")
    float BoostedThrust = 300.0f; // Applied when correct answer is given

    UPROPERTY(EditAnywhere, Category = "Flight")
    float FuelDrainIdle = 1.0f; // Fuel per second at default thrust

    UPROPERTY(EditAnywhere, Category = "Flight")
    float FuelDrainBoost = 5.0f; // Fuel per second during boost

    UPROPERTY(EditAnywhere, Category = "Flight")
    float BoostDuration = 3.0f; // How long a correct answer boost lasts in seconds

    UPROPERTY(EditAnywhere, Category = "Flight")
    float DefaultForwardThrust = 80.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float BoostedForwardThrust = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float RotationStep = 15.0f; // How many degrees each correct answer rotates

    UPROPERTY(EditAnywhere, Category = "Camera")
    float RotationInterpSpeed = 3.0f; // Higher = faster rotation, lower = slower

    FRotator TargetRotation = FRotator::ZeroRotator;

    void RotateLeft();
    void RotateRight();

    bool bForwardThrustMode = false;

    void ToggleThrustMode();

    float CurrentBoostTimer = 0.0f; // Tracks remaining boost time
    bool bIsBoosting = false;

    float LowGravity = -160.0f;
    float HighGravity = -980.0f;

    void ActivateBoost();

    void AddFuel(float Amount);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};