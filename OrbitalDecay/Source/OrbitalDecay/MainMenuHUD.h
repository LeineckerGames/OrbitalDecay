#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

UCLASS()
class ORBITALDECAY_API AMainMenuHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void BeginPlay() override;

private:
    TSharedPtr<class SMainMenuWidget> MainMenuWidget;
};