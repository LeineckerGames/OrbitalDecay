#include "OrbitalSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString UOrbitalSaveGame::SaveSlotName = TEXT("OrbitalDecayScores");

void UOrbitalSaveGame::AddScore(int32 Level, const FScoreEntry& Entry)
{
    TArray<FScoreEntry>& Scores = LevelScores.FindOrAdd(Level).Entries;
    Scores.Add(Entry);

    Scores.Sort([](const FScoreEntry& A, const FScoreEntry& B)
    {
        return A.CompletionTimeSeconds < B.CompletionTimeSeconds;
    });

    if (Scores.Num() > MaxScoresPerLevel)
        Scores.SetNum(MaxScoresPerLevel);

    UGameplayStatics::SaveGameToSlot(this, SaveSlotName, 0);
}

TArray<FScoreEntry> UOrbitalSaveGame::GetScores(int32 Level) const
{
    const FScoreList* Found = LevelScores.Find(Level);
    return Found ? Found->Entries : TArray<FScoreEntry>();
}

//---------------------------For Level tracking----------------------------------
void UOrbitalSaveGame::SaveCurrentLevel(int32 Level)
{
    CurrentLevel = Level;
    UGameplayStatics::SaveGameToSlot(this, SaveSlotName, 0);
}

int32 UOrbitalSaveGame::LoadCurrentLevel() const
{
    return CurrentLevel;
}

void UOrbitalSaveGame::ResetToLevelOne()
{
    CurrentLevel = 1;
    UGameplayStatics::SaveGameToSlot(this, SaveSlotName, 0);
}
//-------------------------------------------------------------------------------

void UOrbitalSaveGame::SetTutorialRun(bool bValue)
{
    bIsTutorialRun = bValue;
    UGameplayStatics::SaveGameToSlot(this, SaveSlotName, 0);
}

bool UOrbitalSaveGame::IsTutorialRun() const
{
    return bIsTutorialRun;
}