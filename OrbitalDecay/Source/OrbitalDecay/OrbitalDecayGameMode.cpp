#include "OrbitalDecayGameMode.h"
#include "MyHUD.h"
#include "ALanderPawn.h"
#include "Kismet/GameplayStatics.h"

AOrbitalDecayGameMode::AOrbitalDecayGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PawnBP(TEXT("/Game/models/BP_LanderPawn"));
	if (PawnBP.Class != nullptr)
	{
    		DefaultPawnClass = PawnBP.Class;
	}

	// This "wires" = custom HUD class to the GameMode
	HUDClass = AMyHUD::StaticClass();
}

void AOrbitalDecayGameMode::BeginPlay()
{
    Super::BeginPlay();

    //For level tracking
    //Load saved level so it persists across reloads
    UOrbitalSaveGame* SaveGame = GetOrCreateSaveGame();
    GlobalLevel = SaveGame->LoadCurrentLevel();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReplayRecorder::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        ReplayRecorder = Cast<AReplayRecorder>(FoundActors[0]);
    }

    SetGameState(EGameState::Playing);
}

void AOrbitalDecayGameMode::SetGameState(EGameState NewState)
{
    CurrentGameState = NewState;

    switch (CurrentGameState)
    {
    case EGameState::Playing:
        // TODO: re-enable ALanderPawn input
        // TODO: restart MathGenerator
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Playing"));
        break;

    case EGameState::Success:
        // TODO: disable ALanderPawn input
        // TODO: show success UI on AMyHUD
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Success"));
        StartReplay();
        break;

    case EGameState::Failure:
    {
        AMyHUD* HUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
        if (HUD) HUD->ShowFailureScreen();
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Failure"));
        StartReplay();
        break;
    }

    case EGameState::Replay:
        if (ReplayRecorder)
        {
            ReplayRecorder->StartReplay();
        }
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Replay"));
        break;
    }
}

//---------------------------For Level tracking----------------------------------
UOrbitalSaveGame* AOrbitalDecayGameMode::GetOrCreateSaveGame()
{
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(UOrbitalSaveGame::SaveSlotName, 0));

    if (!SaveGame)
    {
        SaveGame = Cast<UOrbitalSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UOrbitalSaveGame::StaticClass()));
        UGameplayStatics::SaveGameToSlot(SaveGame, UOrbitalSaveGame::SaveSlotName, 0);
    }
    return SaveGame;
}

void AOrbitalDecayGameMode::TriggerLevelComplete()
{
    UOrbitalSaveGame* SaveGame = GetOrCreateSaveGame();

    if (GlobalLevel >= 20)
    {
        // All levels done � reset back to 1 for next playthrough
        SaveGame->ResetToLevelOne();
        return;
    }

    // Increment and save so the next BeginPlay loads the right level
    GlobalLevel++;
    SaveGame->SaveCurrentLevel(GlobalLevel);
}
//-------------------------------------------------------------------------------

void AOrbitalDecayGameMode::TriggerSuccess() { SetGameState(EGameState::Success); }
void AOrbitalDecayGameMode::TriggerFailure() { SetGameState(EGameState::Failure); }
void AOrbitalDecayGameMode::StartReplay() { SetGameState(EGameState::Replay); }
void AOrbitalDecayGameMode::RestartGame() { /* Sprint 5 */ }