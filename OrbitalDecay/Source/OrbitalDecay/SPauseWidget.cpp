#include "SPauseWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitalSaveGame.h"

static const FLinearColor PW_Bg      = FLinearColor(0.02f, 0.03f, 0.05f, 0.92f);
static const FLinearColor PW_Panel   = FLinearColor(0.05f, 0.07f, 0.10f, 1.f);
static const FLinearColor PW_Border  = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor PW_BtnBg   = FLinearColor(0.08f, 0.12f, 0.18f, 1.f);
static const FLinearColor PW_BtnRed  = FLinearColor(0.25f, 0.05f, 0.05f, 1.f);
static const FLinearColor PW_Title   = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor PW_Text    = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);

static TSharedRef<SWidget> MakePauseButton(
    const FString& Label,
    FLinearColor BgColor,
    FOnClicked OnClicked,
    FSimpleDelegate OnHovered = FSimpleDelegate())
{
    return SNew(SBox)
        .WidthOverride(240.f)
        .HeightOverride(52.f)
        [
            SNew(SButton)
            .ButtonColorAndOpacity(BgColor)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .OnClicked(OnClicked)
            .OnHovered(OnHovered)
            [
                SNew(STextBlock)
                .Text(FText::FromString(Label))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
                .ColorAndOpacity(PW_Text)
                .Justification(ETextJustify::Center)
            ]
        ];
}

void SPauseWidget::Construct(const FArguments& InArgs)
{
    MyWorld  = InArgs._OwnerWorld;
    OnResume = InArgs._OnResume;

    ButtonClickSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/342200__christopherderp__videogame-menu-button-click.342200__christopherderp__videogame-menu-button-click"));
    ButtonHoverSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/ButtonHover.ButtonHover"));

    ChildSlot
    [
        // Full screen semi-transparent dark overlay
        SNew(SOverlay)

        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(PW_Bg)
        ]

        // Centered pause panel
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .WidthOverride(320.f)
            .HeightOverride(340.f)
            [
                SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor(PW_Panel)
                .Padding(30.f)
                [
                    SNew(SVerticalBox)

                    // PAUSED title
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 0.f, 0.f, 24.f)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("PAUSED")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 36))
                        .ColorAndOpacity(PW_Title)
                        .Justification(ETextJustify::Center)
                    ]

                    // Resume
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 6.f)
                    [
                        MakePauseButton(TEXT("RESUME"), PW_BtnBg,
                            FOnClicked::CreateSP(this, &SPauseWidget::OnResumeClicked),
                            FSimpleDelegate::CreateSP(this, &SPauseWidget::PlayHoverSound))
                    ]

                    // Restart
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 6.f)
                    [
                        MakePauseButton(TEXT("RESTART"), PW_BtnBg,
                            FOnClicked::CreateSP(this, &SPauseWidget::OnRestartClicked),
                            FSimpleDelegate::CreateSP(this, &SPauseWidget::PlayHoverSound))
                    ]

                    // Main Menu
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0.f, 6.f)
                    [
                        MakePauseButton(TEXT("MAIN MENU"), PW_BtnRed,
                            FOnClicked::CreateSP(this, &SPauseWidget::OnMainMenuClicked),
                            FSimpleDelegate::CreateSP(this, &SPauseWidget::PlayHoverSound))
                    ]
                ]
            ]
        ]
    ];
}

void SPauseWidget::PlayButtonSound()
{
    if (ButtonClickSound && MyWorld)
        UGameplayStatics::PlaySound2D(MyWorld, ButtonClickSound);
}

void SPauseWidget::PlayHoverSound()
{
    if (ButtonHoverSound && MyWorld)
        UGameplayStatics::PlaySound2D(MyWorld, ButtonHoverSound, 0.5f);
}

FReply SPauseWidget::OnResumeClicked()
{
    PlayButtonSound();
    // Unpause the game
    APlayerController* PC = MyWorld ?
        MyWorld->GetFirstPlayerController() : nullptr;
    if (PC) PC->SetPause(false);

    // Notify HUD to hide this widget
    OnResume.ExecuteIfBound();
    return FReply::Handled();
}

FReply SPauseWidget::OnRestartClicked()
{
    PlayButtonSound();
    APlayerController* PC = MyWorld ?
        MyWorld->GetFirstPlayerController() : nullptr;
    if (PC) PC->SetPause(false);

    if (MyWorld)
    {
        TSharedRef<SPauseWidget> Self = SharedThis(this);
        FTimerHandle TimerHandle;
        MyWorld->GetTimerManager().SetTimer(TimerHandle, [this, Self]()
        {
            if (GEngine && GEngine->GameViewport)
                GEngine->GameViewport->RemoveAllViewportWidgets();

            if (MyWorld)
            {
                UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
                    UGameplayStatics::LoadGameFromSlot(
                        UOrbitalSaveGame::SaveSlotName, 0));
                if (SaveGame)
                    SaveGame->ResetToLevelOne();

                UGameplayStatics::OpenLevel(MyWorld,
                    FName(*UGameplayStatics::GetCurrentLevelName(MyWorld)));
            }
        }, 0.2f, false);
    }

    return FReply::Handled();
}

FReply SPauseWidget::OnMainMenuClicked()
{
    PlayButtonSound();
    APlayerController* PC = MyWorld ?
        MyWorld->GetFirstPlayerController() : nullptr;
    if (PC) PC->SetPause(false);

    if (MyWorld)
    {
        TSharedRef<SPauseWidget> Self = SharedThis(this);
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