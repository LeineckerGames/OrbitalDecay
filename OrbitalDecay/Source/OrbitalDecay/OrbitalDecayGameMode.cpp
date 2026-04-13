#include "OrbitalDecayGameMode.h"
#include "MyHUD.h"
#include "ALanderPawn.h"

AOrbitalDecayGameMode::AOrbitalDecayGameMode()
{
	DefaultPawnClass = ALanderPawn::StaticClass();

	// This "wires" = custom HUD class to the GameMode
	HUDClass = AMyHUD::StaticClass();
}