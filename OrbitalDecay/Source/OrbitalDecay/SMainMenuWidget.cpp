#include "SMainMenuWidget.h"
#include "SHighScoreWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Misc/App.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "OrbitalSaveGame.h"

// ─── Colors ───────────────────────────────────────────────────────
static const FLinearColor MM_Bg         = FLinearColor(0.02f, 0.03f, 0.05f, 1.f);
static const FLinearColor MM_Panel      = FLinearColor(0.05f, 0.07f, 0.10f, 1.f);
static const FLinearColor MM_Border     = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor MM_BtnBg      = FLinearColor(0.08f, 0.12f, 0.18f, 1.f);
static const FLinearColor MM_BtnHover   = FLinearColor(0.15f, 0.22f, 0.32f, 1.f);
static const FLinearColor MM_TitleColor = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor MM_TextColor  = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor MM_SubColor   = FLinearColor(0.50f, 0.60f, 0.70f, 1.f);

// ─── Helper: styled menu button ──────────────────────────────────
static TSharedRef<SWidget> MakeMenuButton(
    const FString& Label,
    FOnClicked OnClicked)
{
    return SNew(SBox)
        .WidthOverride(280.f)
        .HeightOverride(55.f)
        [
            SNew(SButton)
            .ButtonColorAndOpacity(MM_BtnBg)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .OnClicked(OnClicked)
            [
                SNew(STextBlock)
                .Text(FText::FromString(Label))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
                .ColorAndOpacity(MM_TextColor)
                .Justification(ETextJustify::Center)
            ]
        ];
}

// ─── Helper: large green Play button ──────────────────────────────
static const FLinearColor MM_PlayBg = FLinearColor(0.08f, 0.32f, 0.10f, 1.f);

static TSharedRef<SWidget> MakePlayButton(FOnClicked OnClicked)
{
    return SNew(SBox)
        .WidthOverride(320.f)
        .HeightOverride(72.f)
        [
            SNew(SButton)
            .ButtonColorAndOpacity(MM_PlayBg)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .OnClicked(OnClicked)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("PLAY")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 28))
                .ColorAndOpacity(MM_TextColor)
                .Justification(ETextJustify::Center)
            ]
        ];
}

// ─── Construct ───────────────────────────────────────────────────
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
    MyWorld = InArgs._OwnerWorld;

    ButtonClickSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/342200__christopherderp__videogame-menu-button-click.342200__christopherderp__videogame-menu-button-click"));

    ContentArea = SNew(SBox);

    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(MM_Bg)
        [
            SNew(SOverlay)

            // Full background
            + SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SSpacer)
            ]

            // Content area - starts with home page
            + SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                ContentArea.ToSharedRef()
            ]
        ]
    ];

    // Start on home page
    NavigateTo(BuildHomePage());
}

// ─── Navigate ────────────────────────────────────────────────────
void SMainMenuWidget::NavigateTo(TSharedRef<SWidget> NewPage)
{
    ContentArea->SetContent(NewPage);
}

// ─── Home Page ───────────────────────────────────────────────────
TSharedRef<SWidget> SMainMenuWidget::BuildHomePage()
{
    return SNew(SVerticalBox)

        + SVerticalBox::Slot()
        .FillHeight(0.35f)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Bottom)
        .Padding(0, 0, 0, 20)
        [
            SNew(SVerticalBox)

            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("ORBITAL DECAY")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 64))
                .ColorAndOpacity(MM_TitleColor)
                .Justification(ETextJustify::Center)
            ]

            // Subtitle
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0, 8)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("A Math-Powered Space Lander")))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
                .ColorAndOpacity(MM_SubColor)
                .Justification(ETextJustify::Center)
            ]
        ]

        // Buttons
        + SVerticalBox::Slot()
        .FillHeight(0.65f)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Top)
        .Padding(0, 20)
        [
            SNew(SVerticalBox)

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8, 0, 16)
            [
                MakePlayButton(
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnStartClicked))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("TUTORIAL"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnTutorialClicked))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("HIGH SCORES"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnHighScoresClicked))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("ABOUT"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnAboutClicked))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("QUIT"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnQuitClicked))
            ]
        ];
}



// ─── High Scores Page ────────────────────────────────────────────
TSharedRef<SWidget> SMainMenuWidget::BuildHighScoresPage()
{
    return SNew(SHighScoreWidget)
        .OwnerWorld(MyWorld)
        .OnBack_Lambda([this]()
        {
            NavigateTo(BuildHomePage());
        });
}

// ─── Tutorial Page ────────────────────────────────────────────────
TSharedRef<SWidget> SMainMenuWidget::BuildTutorialPage()
{
    // Load your tutorial image texture
    UTexture2D* TutorialTex = LoadObject<UTexture2D>(nullptr, 
        TEXT("/Game/images/Slide1"));  // adjust path to where you save it

    FSlateBrush* TutorialBrush = new FSlateBrush();
    if (TutorialTex)
    {
        TutorialBrush->SetResourceObject(TutorialTex);
        TutorialBrush->ImageSize = FVector2D(1920.f, 1080.f);
        TutorialBrush->DrawAs = ESlateBrushDrawType::Image;
    }

    return SNew(SOverlay)

        // Full screen image
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SImage)
            .Image(TutorialBrush)
        ]

        // Back button overlaid in bottom left
        + SOverlay::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Bottom)
        .Padding(30.f)
        [
            SNew(SBox)
            .WidthOverride(120.f)
            .HeightOverride(44.f)
            [
                SNew(SButton)
                .ButtonColorAndOpacity(MM_BtnBg)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked(FOnClicked::CreateLambda([this]() -> FReply
                {
                    NavigateTo(BuildHomePage());
                    return FReply::Handled();
                }))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("← BACK")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                    .ColorAndOpacity(MM_TextColor)
                ]
            ]
        ];
}

// ─── About Page ───────────────────────────────────────────────────
TSharedRef<SWidget> SMainMenuWidget::BuildAboutPage()
{
    return SNew(SVerticalBox)

        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 40)
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("ABOUT")))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
            .ColorAndOpacity(MM_TitleColor)
        ]

        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 20)
        [
            SNew(SBox)
            .WidthOverride(600.f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("This is a game.")))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
                .ColorAndOpacity(MM_TextColor)
                .AutoWrapText(true)
                .Justification(ETextJustify::Center)
            ]
        ]

        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 40)
        [
            MakeMenuButton(TEXT("BACK"),
                FOnClicked::CreateLambda([this]() -> FReply
                {
                    NavigateTo(BuildHomePage());
                    return FReply::Handled();
                }))
        ];
}

// ─── Button Handlers ─────────────────────────────────────────────
FReply SMainMenuWidget::OnStartClicked()
{
    PlayButtonSound();

    // Always start a fresh playthrough at level 1 — protects
    // against stale progress if the player exits without using
    // Main Menu (Alt+F4, editor stop, crash, etc.)
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(
            UOrbitalSaveGame::SaveSlotName, 0));
    if (!SaveGame)
        SaveGame = Cast<UOrbitalSaveGame>(
            UGameplayStatics::CreateSaveGameObject(
                UOrbitalSaveGame::StaticClass()));

    if (SaveGame)
    {
        SaveGame->SaveCurrentLevel(1);
        UGameplayStatics::SaveGameToSlot(
            SaveGame, UOrbitalSaveGame::SaveSlotName, 0);
    }

    if (MyWorld)
    {
        // "test" is the current gameplay level while LevelCreation
        // is being built — swap the FName here once that level exists
        UGameplayStatics::OpenLevel(MyWorld, FName("test"));
    }

    return FReply::Handled();
}

FReply SMainMenuWidget::OnTutorialClicked()
{
    PlayButtonSound();
    NavigateTo(BuildTutorialPage());
    return FReply::Handled();
}

FReply SMainMenuWidget::OnHighScoresClicked()
{
    PlayButtonSound();
    NavigateTo(BuildHighScoresPage());
    return FReply::Handled();
}

FReply SMainMenuWidget::OnAboutClicked()
{
    PlayButtonSound();
    NavigateTo(BuildAboutPage());
    return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked()
{
    PlayButtonSound();

    FTimerHandle QuitTimer;
    if (MyWorld)
    {
        MyWorld->GetTimerManager().SetTimer(QuitTimer, [this]()
            {
                if (GEngine)
                {
                    APlayerController* PC = GEngine->GetFirstLocalPlayerController(MyWorld);
                    if (PC)
                    {
                        PC->ConsoleCommand("quit");
                    }
                }
            }, 0.3f, false);
    }

    return FReply::Handled();
}

void SMainMenuWidget::PlayButtonSound()
{
    if (ButtonClickSound && MyWorld)
    {
        UGameplayStatics::PlaySound2D(MyWorld, ButtonClickSound);
    }
}