#include "SCrashScreen.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"


void SCrashScreen::Construct(const FArguments& InArgs)
{
    MyWorld = InArgs._OwnerWorld;

    ChildSlot
    [
        // Full screen dark overlay
        SNew(SOverlay)

        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.75f))
        ]

        // Centered content
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(400.f)
            .HeightOverride(250.f)
            [
                SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor(FLinearColor(0.08f, 0.05f, 0.05f, 1.f))
                .Padding(30.f)
                [
                    SNew(SVerticalBox)

                    // CRASHED title
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 0.f, 0.f, 16.f)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("CRASHED")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
                        .ColorAndOpacity(FLinearColor(0.95f, 0.15f, 0.10f, 1.f))
                        .Justification(ETextJustify::Center)
                    ]

                    // Subtitle
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 0.f, 0.f, 30.f)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("You missed the landing pad")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                        .ColorAndOpacity(FLinearColor(0.75f, 0.75f, 0.75f, 1.f))
                        .Justification(ETextJustify::Center)
                    ]

                    // RETRY button
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    [
                        SNew(SBox)
                        .WidthOverride(200.f)
                        .HeightOverride(55.f)
                        [
                            SNew(SButton)
                            .ButtonColorAndOpacity(FLinearColor(0.15f, 0.20f, 0.25f, 1.f))
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            .OnClicked(this, &SCrashScreen::OnRetryClicked)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(TEXT("RETRY")))
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
                                .ColorAndOpacity(FLinearColor(0.85f, 0.90f, 1.00f, 1.f))
                                .Justification(ETextJustify::Center)
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];
}

FReply SCrashScreen::OnRetryClicked()
{
    if (MyWorld)
    {
        // Remove this widget from the viewport before reloading
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->RemoveViewportWidgetContent(
                SharedThis(this));
        }

        // Do NOT reset level progress on crash — the player should
        // retry the SAME level they just failed, not go back to 1.
        UGameplayStatics::OpenLevel(MyWorld,
            FName(*UGameplayStatics::GetCurrentLevelName(MyWorld)));
    }
    return FReply::Handled();
}