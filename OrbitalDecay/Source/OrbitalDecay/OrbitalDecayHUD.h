#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OrbitalDecayHUD.generated.h"

UCLASS()
class ORBITALDECAY_API AOrbitalDecayHUD : public AHUD
{
    GENERATED_BODY()

public:
    AOrbitalDecayHUD();
    virtual void BeginPlay() override;

private:
    TSubclassOf<class UUserWidget> CockpitWidgetClass;
};