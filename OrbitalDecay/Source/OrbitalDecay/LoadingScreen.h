#pragma once
#include "CoreMinimal.h"

class ULoadingScreen
{
public:
    // Show a full-screen loading overlay then immediately open the
    // specified level. The overlay disappears naturally when the new
    // level finishes loading and destroys the current world.
    static void Show(UWorld* World, FName LevelName);
};