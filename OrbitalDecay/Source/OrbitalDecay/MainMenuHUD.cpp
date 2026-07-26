#include "MainMenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Framework/Application/SlateApplication.h"

void AMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    // Clear any widgets that persisted from the previous world
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveAllViewportWidgets();
    }

    if (GEngine && GEngine->GameViewport)
    {
        MainMenuWidget = SNew(SMainMenuWidget).OwnerWorld(GetWorld());
        GEngine->GameViewport->AddViewportWidgetContent(
            SNew(SWeakWidget).PossiblyNullContent(MainMenuWidget.ToSharedRef()),
            0);

        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuWidget);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }

        FSlateApplication::Get().SetKeyboardFocus(MainMenuWidget);
    }
}