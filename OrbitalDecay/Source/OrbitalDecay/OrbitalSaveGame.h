#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbitalSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FScoreEntry
{
    GENERATED_BODY()

    UPROPERTY()
    FString PlayerName = TEXT("");

    UPROPERTY()
    float CompletionTimeSeconds = 0.f;

    UPROPERTY()
    float AccuracyPercent = 0.f;

    UPROPERTY()
    FString DateString = TEXT("");
};

// Wrapper struct needed because TMap can't have TArray as value in UPROPERTY
USTRUCT()
struct FScoreList
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FScoreEntry> Entries;
};

UCLASS()
class ORBITALDECAY_API UOrbitalSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    // Key = level number (1-20), Value = wrapper containing top 10 scores
    UPROPERTY()
    TMap<int32, FScoreList> LevelScores;

    void AddScore(int32 Level, const FScoreEntry& Entry);
    TArray<FScoreEntry> GetScores(int32 Level) const;

    static const FString SaveSlotName;
    static const int32   MaxScoresPerLevel = 10;

    //For Level tracking
    UPROPERTY()
    int32 CurrentLevel = 1;
    UPROPERTY()
    bool bIsTutorialRun = false;

    void SaveCurrentLevel(int32 Level);
    int32 LoadCurrentLevel() const;
    void ResetToLevelOne();
    void SetTutorialRun(bool bValue);
    bool IsTutorialRun() const;
};