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
        // TODO: disable ALanderPawn input
        // TODO: show failure UI on AMyHUD
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Failure"));
        StartReplay();
        break;

    case EGameState::Replay:
        if (ReplayRecorder)
        {
            ReplayRecorder->StartReplay();
        }
        UE_LOG(LogTemp, Warning, TEXT("GameState -> Replay"));
        break;
    }
}

void AOrbitalDecayGameMode::TriggerSuccess() { SetGameState(EGameState::Success); }
void AOrbitalDecayGameMode::TriggerFailure() { SetGameState(EGameState::Failure); }
void AOrbitalDecayGameMode::StartReplay() { SetGameState(EGameState::Replay); }
void AOrbitalDecayGameMode::RestartGame() { /* Sprint 5 */ }