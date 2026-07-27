#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ReplayRecorder.h"
#include "MissionCharacter.h"
#include "Components/AudioComponent.h"
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

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USpringArmComponent* ReplaySpringArm;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* ReplayCamera;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USceneCaptureComponent2D* BottomCamera;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UTextureRenderTarget2D* BottomCameraRenderTarget;

    UPROPERTY(EditAnywhere, Category = "Camera")
    bool bShowBottomCamera = true;

    UPROPERTY(EditAnywhere, Category = "Replay")
    AReplayRecorder* ReplayRecorder = nullptr;

    UPROPERTY(EditAnywhere, Category = "Flight")
    int32 MyCurrentLevel = 1;

    UPROPERTY(EditAnywhere, Category = "Flight")
    FVector CurrentVelocity = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float CurrentAltitude = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float Fuel = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float MaxFuel = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float DefaultThrust = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float BoostedThrust = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float FuelDrainIdle = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float FuelDrainBoost = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float VerticalBoostDuration = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float ForwardBoostDuration = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float DefaultForwardThrust = 80.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float BoostedForwardThrust = 800.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float RotationStep = 15.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float RotationInterpSpeed = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float MaxSafeLandingVelocity = -200.0f;

    UPROPERTY(EditAnywhere, Category = "Mission")
    float MissionBriefingDuration = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Mission")
    TArray<UMissionCharacter*> MissionCharacters;

    UPROPERTY(BlueprintReadOnly, Category = "Mission")
    UMissionCharacter* SelectedCharacter;

    TSet<AActor*> VisitedPads;
    int32 PadsLanded = 0;
    bool bLevelComplete = false;

    bool bGameStarted = false;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float AirResistance = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")

    FRotator CameraRotation = FRotator(-15.f, 0.f, 0.f);

    FRotator TargetRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* ThrustSound = nullptr;

    UPROPERTY()
    UAudioComponent* ThrustAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* RotationSound = nullptr;

    UPROPERTY()
    UAudioComponent* RotationAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* KeyClickSound = nullptr;

    UPROPERTY()
    UAudioComponent* KeyClickAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* SwitchSound = nullptr;

    UPROPERTY()
    UAudioComponent* SwitchAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* LowFuelSound = nullptr;

    UPROPERTY()
    UAudioComponent* LowFuelAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* CorrectAnswerSound = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* WrongAnswerSound = nullptr;

    UPROPERTY()
    UAudioComponent* CorrectAnswerAudioComponent = nullptr;

    UPROPERTY()
    UAudioComponent* WrongAnswerAudioComponent = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* FuelSound = nullptr;

    UPROPERTY()
    UAudioComponent* FuelAudioComponent = nullptr;

    void RotateLeft();
    void RotateRight();
    void ToggleThrustMode();
    void ToggleBottomCamera();
    void SetBottomCameraActive(bool bActive);
    void ActivateBoost();
    void AddFuel(float Amount);
    void StartCrashReplay();
    void TriggerCrash();
    void DisablePlayerInput();
    void ResetPawn(); 
    void PlayKeyClickSound();
    void PlaySwitchSound();
    void PlayFuelSound();
    void PlayCorrectAnswerSound();
    void PlayWrongAnswerSound();

    bool bForwardThrustMode = false;
    bool bHasLanded = false;
    bool bIsBoosting = false;
    bool bAutoDescending = false;

    UPROPERTY(EditAnywhere, Category = "Flight")
    float AutoDescendSpeed = 500.0f;

    // How close to the pad's centre (in cm, XY only) the trace must land
    // before auto-descent kicks in.  Increase if descent triggers too easily,
    // decrease if players have to be too precise.
    UPROPERTY(EditAnywhere, Category = "Flight")
    float PadCenterRadius = 300.0f;

    float CurrentBoostTimer = 0.0f;
    float LowGravity = -160.0f;
    float HighGravity = -300.0f;

    float LevelTimeSeconds = 0.f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};