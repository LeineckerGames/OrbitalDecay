#include "STutorialOverlay.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"

void STutorialOverlay::Construct(const FArguments& InArgs)
{
    MyWorld = InArgs._OwnerWorld;
    OnDismissed = InArgs._OnDismissed;

    FString Title = InArgs._TitleText;
    FString Body = InArgs._BodyText;

    ChildSlot
        [
            SNew(SOverlay)

                // Semi-transparent dark background
                + SOverlay::Slot()
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                [
                    SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.6f))
                ]

                // Centered panel
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SBox)
                        .WidthOverride(620.f)
                        [
                            SNew(SBorder)
                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                .BorderBackgroundColor(FLinearColor(0.04f, 0.07f, 0.05f, 1.f))
                                .Padding(36.f)
                                [
                                    SNew(SVerticalBox)

                                        // Title
                                        + SVerticalBox::Slot()
                                        .AutoHeight()
                                        .HAlign(HAlign_Center)
                                        .Padding(0, 0, 0, 16)
                                        [
                                            SNew(STextBlock)
                                                .Text(FText::FromString(Title))
                                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
                                                .ColorAndOpacity(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                                                .Justification(ETextJustify::Center)
                                        ]

                                    // Body
                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        .HAlign(HAlign_Center)
                                        .Padding(0, 0, 0, 28)
                                        [
                                            SNew(SBox)
                                                .WidthOverride(540.f)
                                                [
                                                    SNew(STextBlock)
                                                        .Text(FText::FromString(Body))
                                                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))
                                                        .ColorAndOpacity(FLinearColor(0.75f, 0.80f, 0.90f, 1.f))
                                                        .Justification(ETextJustify::Center)
                                                        .AutoWrapText(true)
                                                ]
                                        ]

                                    // Got it button
                                    + SVerticalBox::Slot()
                                        .AutoHeight()
                                        .HAlign(HAlign_Center)
                                        [
                                            SNew(SBox)
                                                .WidthOverride(180.f)
                                                .HeightOverride(50.f)
                                                [
                                                    SNew(SButton)
                                                        .ButtonColorAndOpacity(FLinearColor(0.08f, 0.28f, 0.10f, 1.f))
                                                        .HAlign(HAlign_Center)
                                                        .VAlign(VAlign_Center)
                                                        .OnClicked(this, &STutorialOverlay::OnGotItClicked)
                                                        [
                                                            SNew(STextBlock)
                                                                .Text(FText::FromString(TEXT("GOT IT")))
                                                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
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

FReply STutorialOverlay::OnGotItClicked()
{
    if (GEngine && GEngine->GameViewport)
        GEngine->GameViewport->RemoveViewportWidgetContent(SharedThis(this));

    // Unpause the game.  Input mode and cursor are restored by the
    // OnDismissed callback in AMyHUD, which also re-focuses the cockpit.
    if (MyWorld)
    {
        APlayerController* PC = MyWorld->GetFirstPlayerController();
        if (PC)
        {
            PC->SetPause(false);
            PC->bShowMouseCursor = true;
        }
    }

    // Notify the HUD that this step was dismissed (re-enables cockpit, etc.)
    OnDismissed.ExecuteIfBound();

    return FReply::Handled();
}