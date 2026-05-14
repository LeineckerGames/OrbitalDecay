#include "OrbitalDecayGameMode.h"
#include "MyHUD.h"
#include "ALanderPawn.h"

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