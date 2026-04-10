#include "OrbitalDecayGameMode.h"
#include "MyHUD.h"
#include "ALanderPawn.h"

AHUDSetup2GameMode::AHUDSetup2GameMode()
{
	DefaultPawnClass = ALanderPawn::StaticClass();

	// This "wires" = custom HUD class to the GameMode
	HUDClass = AMyHUD::StaticClass();
}