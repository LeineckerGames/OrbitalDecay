#include "LoadingScreen.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"

void ULoadingScreen::Show(UWorld* World, FName LevelName)
{
    if (!World || !GEngine || !GEngine->GameViewport) return;

    // Clear any existing viewport widgets before showing loading screen
    GEngine->GameViewport->RemoveAllViewportWidgets();

    TSharedRef<SWidget> LoadingWidget =
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(FLinearColor::Black)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SVerticalBox)

            + SVerticalBox::Slot()
            .FillHeight(1.0f)

            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0.f, 0.f, 0.f, 16.f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("LOADING...")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 32))
                .ColorAndOpacity(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                .Justification(ETextJustify::Center)
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("Please wait...")))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                .ColorAndOpacity(FLinearColor(0.50f, 0.60f, 0.70f, 1.f))
                .Justification(ETextJustify::Center)
            ]

            + SVerticalBox::Slot()
            .FillHeight(1.0f)
        ];

    GEngine->GameViewport->AddViewportWidgetContent(LoadingWidget, 20);

    // Force a render so the loading screen is actually visible
    // before OpenLevel blocks the game thread
    FSlateApplication::Get().PumpMessages();
    GEngine->GameViewport->GetWindow()->Invalidate(
        EInvalidateWidgetReason::Paint);
    FSlateApplication::Get().Tick();
    FSlateApplication::Get().GetRenderer()->Sync();

    // Open the level — blocks until complete
    UGameplayStatics::OpenLevel(World, LevelName);
}