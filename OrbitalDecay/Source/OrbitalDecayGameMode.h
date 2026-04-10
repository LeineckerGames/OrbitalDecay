#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitalDecayGameMode.generated.h"

UCLASS()
class ORBITALDECAY_API AHUDSetup2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHUDSetup2GameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Progress")
	int32 GlobalLevel = 1;
};
