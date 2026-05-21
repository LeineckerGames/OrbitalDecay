#include "MainMenuGameMode.h"
#include "MainMenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    HUDClass = AMainMenuHUD::StaticClass();
}