#include "OrbitalDecayHUD.h"
#include "CockpitWidget.h"
#include "Blueprint/UserWidget.h"

AOrbitalDecayHUD::AOrbitalDecayHUD()
{
    // Set the widget class directly to our C++ CockpitWidget
    CockpitWidgetClass = UCockpitWidget::StaticClass();
}

void AOrbitalDecayHUD::BeginPlay()
{
    Super::BeginPlay();

    if (CockpitWidgetClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CockpitWidgetClass);

        if (Widget)
        {
            Widget->AddToViewport();
        }
    }
}