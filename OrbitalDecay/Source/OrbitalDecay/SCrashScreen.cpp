#include "SCrashScreen.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"

static const FLinearColor CS_Bg      = FLinearColor(0.00f, 0.00f, 0.00f, 0.80f);
static const FLinearColor CS_Panel   = FLinearColor(0.06f, 0.04f, 0.04f, 1.f);
static const FLinearColor CS_Border  = FLinearColor(0.20f, 0.10f, 0.10f, 1.f);
static const FLinearColor CS_Title   = FLinearColor(0.95f, 0.15f, 0.10f, 1.f);
static const FLinearColor CS_Sub     = FLinearColor(0.65f, 0.55f, 0.55f, 1.f);
static const FLinearColor CS_BtnBg   = FLinearColor(0.10f, 0.12f, 0.18f, 1.f);
static const FLinearColor CS_BtnRed  = FLinearColor(0.25f, 0.05f, 0.05f, 1.f);
static const FLinearColor CS_Text    = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);

void SCrashScreen::Construct(const FArguments& InArgs)
{
    MyWorld      = InArgs._OwnerWorld;
    CurrentLevel = InArgs._CurrentLevel;

    ButtonClickSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/342200__christopherderp__videogame-menu-button-click.342200__christopherderp__videogame-menu-button-click"));
    ButtonHoverSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/ButtonHover.ButtonHover"));

    ChildSlot
    [
        SNew(SOverlay)

        // Full screen dark overlay
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(CS_Bg)
        ]

        // Centered panel
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(480.f)
            [
                SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor(CS_Panel)
                .Padding(36.f)
                [
                    SNew(SVerticalBox)

                    // CRASHED title
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 12)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("CRASHED")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 48))
                        .ColorAndOpacity(CS_Title)
                        .Justification(ETextJustify::Center)
                    ]

                    // "You crashed on Level N"
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 32)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(
                            FString::Printf(TEXT("You crashed on Level %d"),
                                CurrentLevel)))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
                        .ColorAndOpacity(CS_Sub)
                        .Justification(ETextJustify::Center)
                    ]

                    // Buttons row — RETRY (left) + MAIN MENU (right)
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    [
                        SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        .Padding(0, 0, 8, 0)
                        [
                            SNew(SBox)
                            .HeightOverride(55.f)
                            [
                                SNew(SButton)
                                .ButtonColorAndOpacity(CS_BtnBg)
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Center)
                                .OnClicked(this, &SCrashScreen::OnRetryClicked)
                                .OnHovered(FSimpleDelegate::CreateSP(this, &SCrashScreen::PlayHoverSound))
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("RETRY")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
                                    .ColorAndOpacity(CS_Text)
                                    .Justification(ETextJustify::Center)
                                ]
                            ]
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        .Padding(8, 0, 0, 0)
                        [
                            SNew(SBox)
                            .HeightOverride(55.f)
                            [
                                SNew(SButton)
                                .ButtonColorAndOpacity(CS_BtnRed)
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Center)
                                .OnClicked(this, &SCrashScreen::OnMainMenuClicked)
                                .OnHovered(FSimpleDelegate::CreateSP(this, &SCrashScreen::PlayHoverSound))
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("MAIN MENU")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
                                    .ColorAndOpacity(CS_Text)
                                    .Justification(ETextJustify::Center)
                                ]
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];
}

void SCrashScreen::PlayButtonSound()
{
    if (ButtonClickSound && MyWorld)
        UGameplayStatics::PlaySound2D(MyWorld, ButtonClickSound);
}

void SCrashScreen::PlayHoverSound()
{
    if (ButtonHoverSound && MyWorld)
        UGameplayStatics::PlaySound2D(MyWorld, ButtonHoverSound, 0.5f);
}

FReply SCrashScreen::OnRetryClicked()
{
    PlayButtonSound();
    if (MyWorld)
    {
        TSharedRef<SCrashScreen> Self = SharedThis(this);
        FTimerHandle TimerHandle;
        MyWorld->GetTimerManager().SetTimer(TimerHandle, [this, Self]()
        {
            if (GEngine && GEngine->GameViewport)
                GEngine->GameViewport->RemoveViewportWidgetContent(SharedThis(this));
            if (MyWorld)
                UGameplayStatics::OpenLevel(MyWorld,
                    FName(*UGameplayStatics::GetCurrentLevelName(MyWorld)));
        }, 0.2f, false);
    }
    return FReply::Handled();
}

FReply SCrashScreen::OnMainMenuClicked()
{
    PlayButtonSound();
    if (MyWorld)
    {
        TSharedRef<SCrashScreen> Self = SharedThis(this);
        FTimerHandle TimerHandle;
        MyWorld->GetTimerManager().SetTimer(TimerHandle, [this, Self]()
        {
            if (GEngine && GEngine->GameViewport)
                GEngine->GameViewport->RemoveAllViewportWidgets();
            if (MyWorld)
                UGameplayStatics::OpenLevel(MyWorld, FName("MainMenu"));
        }, 0.2f, false);
    }
    return FReply::Handled();
}