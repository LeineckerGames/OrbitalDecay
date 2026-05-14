#include "OrbitalDecayGameMode.h"
#include "MyHUD.h"
#include "ALanderPawn.h"

AOrbitalDecayGameMode::AOrbitalDecayGameMode()
{
    DefaultPawnClass = ALanderPawn::StaticClass();
    HUDClass = AMyHUD::StaticClass();
}

void AOrbitalDecayGameMode::BeginPlay()
{
    Super::BeginPlay();
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
        // TODO: disable ALanderPawn input
        // TODO: show failure UI on AMyHUD
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Failure"));
        StartReplay();
        break;

    case EGameState::Replay:
        // TODO: tell ReplayRecorder to play back
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Replay"));
        break;
    }
}

void AOrbitalDecayGameMode::TriggerSuccess() { SetGameState(EGameState::Success); }
void AOrbitalDecayGameMode::TriggerFailure() { SetGameState(EGameState::Failure); }
void AOrbitalDecayGameMode::StartReplay() { SetGameState(EGameState::Replay); }
void AOrbitalDecayGameMode::RestartGame() { /* Sprint 5 */ }