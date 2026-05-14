#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalDecayGameMode.generated.h"
#include "ReplayRecorder.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Playing  UMETA(DisplayName = "Playing"),
    Replay   UMETA(DisplayName = "Replay"),
    Success  UMETA(DisplayName = "Success"),
    Failure  UMETA(DisplayName = "Failure")
};

UCLASS()
class ORBITALDECAY_API AOrbitalDecayGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOrbitalDecayGameMode();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Progress")
    int32 GlobalLevel = 1;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentGameState;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetGameState(EGameState NewState);

    void TriggerSuccess();
    void TriggerFailure();
    void StartReplay();
    void RestartGame(); // stub   Sprint 5

    UPROPERTY()
    AReplayRecorder* ReplayRecorder = nullptr;

protected:
    virtual void BeginPlay() override;
};