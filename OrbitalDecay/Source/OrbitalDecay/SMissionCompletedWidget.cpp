#include "SMissionCompletedWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"

static const FLinearColor MC_Bg      = FLinearColor(0.02f, 0.03f, 0.05f, 0.93f);
static const FLinearColor MC_Panel   = FLinearColor(0.05f, 0.08f, 0.06f, 1.f);
static const FLinearColor MC_BtnRed  = FLinearColor(0.25f, 0.05f, 0.05f, 1.f);
static const FLinearColor MC_Title   = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor MC_Text    = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor MC_SubText = FLinearColor(0.50f, 0.60f, 0.70f, 1.f);

void SMissionCompletedWidget::Construct(const FArguments& InArgs)
{
    MyWorld = InArgs._OwnerWorld;

    ChildSlot
    [
        SNew(SOverlay)

        // Dark overlay background
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(MC_Bg)
        ]

        // Main panel
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(560.f)
            [
                SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor(MC_Panel)
                .Padding(32.f)
                [
                    SNew(SVerticalBox)

                    // MISSION COMPLETED title
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 16)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("MISSION COMPLETED")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 36))
                        .ColorAndOpacity(MC_Title)
                        .Justification(ETextJustify::Center)
                    ]

                    // Subtitle
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 32)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(
                            TEXT("You guided every lander safely across all 20 sectors.")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                        .ColorAndOpacity(MC_SubText)
                        .Justification(ETextJustify::Center)
                        .AutoWrapText(true)
                    ]

                    // Main Menu button
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Fill)
                    [
                        SNew(SBox)
                        .HeightOverride(48.f)
                        [
                            SNew(SButton)
                            .ButtonColorAndOpacity(MC_BtnRed)
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            .OnClicked(this, &SMissionCompletedWidget::OnMainMenuClicked)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(TEXT("MAIN MENU")))
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
                                .ColorAndOpacity(MC_Text)
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];
}

FReply SMissionCompletedWidget::OnMainMenuClicked()
{
    if (GEngine && GEngine->GameViewport)
        GEngine->GameViewport->RemoveAllViewportWidgets();

    if (MyWorld)
        UGameplayStatics::OpenLevel(MyWorld, FName("MainMenu"));

    return FReply::Handled();
}