#include "SMainMenuWidget.h"
#include "OrbitalMenuAssetHolder.h"
#include "EngineUtils.h"
#include "SHighScoreWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Input/SSlider.h"
#include "Misc/App.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "OrbitalSaveGame.h"
#include "OrbitalSettingsSave.h"
#include "LoadingScreen.h"
#include "Widgets/Layout/SUniformGridPanel.h"

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
    FOnClicked OnClicked,
    FSimpleDelegate OnHovered = FSimpleDelegate())
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
            .OnHovered(OnHovered)
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

static TSharedRef<SWidget> MakePlayButton(FOnClicked OnClicked, FSimpleDelegate OnHovered = FSimpleDelegate())
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
            .OnHovered(OnHovered)
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

    // Load persisted settings so sliders remember their last values
    UOrbitalSettingsSave* Settings = Cast<UOrbitalSettingsSave>(
        UGameplayStatics::LoadGameFromSlot(UOrbitalSettingsSave::SaveSlotName, 0));
    if (Settings)
    {
        MusicVolume     = Settings->MusicVolume;
        GameAudioVolume = Settings->GameAudioVolume;
    }

    // Prefer assets from the level-placed holder (guaranteed cooked in packaged
    // builds).  Fall back to LoadObject so the editor still works without it.
    AOrbitalMenuAssetHolder* Holder = nullptr;
    if (MyWorld)
    {
        for (TActorIterator<AOrbitalMenuAssetHolder> It(MyWorld); It; ++It)
        {
            Holder = *It;
            break;
        }
    }

    ButtonClickSound = Holder && Holder->ButtonClickSound
        ? Holder->ButtonClickSound
        : LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/342200__christopherderp__videogame-menu-button-click.342200__christopherderp__videogame-menu-button-click"));
    ButtonHoverSound = Holder && Holder->ButtonHoverSound
        ? Holder->ButtonHoverSound
        : LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/ButtonHover.ButtonHover"));
    MainMenuMusic = Holder && Holder->MainMenuMusic
        ? Holder->MainMenuMusic
        : LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/mainmenumusic.mainmenumusic"));

    // Build background brush fresh every Construct so it is never stale after
    // a level transition (the old static-pointer approach let the UTexture2D
    // get garbage-collected while the brush pointer remained non-null).
    {
        UTexture2D* BgTex = Holder && Holder->BackgroundTexture
            ? Holder->BackgroundTexture
            : LoadObject<UTexture2D>(nullptr, TEXT("/Game/images/loadingscreen"));
        if (BgTex)
        {
            BackgroundBrush.SetResourceObject(BgTex);
            BackgroundBrush.ImageSize = FVector2D(BgTex->GetSizeX(), BgTex->GetSizeY());
            BackgroundBrush.DrawAs = ESlateBrushDrawType::Image;
        }
    }

    if (MainMenuMusic && MyWorld)
    {
        // SpawnSound2D returns the audio component so we can adjust volume live
        // from the settings slider. bAutoDestroy=false keeps it alive until the
        // world tears down when OpenLevel is called.
        MusicComponent = UGameplayStatics::SpawnSound2D(
            MyWorld, MainMenuMusic, MusicVolume, 1.f, 0.f, nullptr, false, false);
    }

    // Apply saved game audio volume immediately so menu button sounds
    // are already at the right level when the menu first appears.
    {
        USoundMix*   Mix   = LoadObject<USoundMix>  (nullptr, TEXT("/Game/Sounds/SM_GameMix.SM_GameMix"));
        USoundClass* Class = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SC_GameAudio.SC_GameAudio"));
        if (Mix && Class && MyWorld)
        {
            UGameplayStatics::PushSoundMixModifier(MyWorld, Mix);
            UGameplayStatics::SetSoundMixClassOverride(MyWorld, Mix, Class, GameAudioVolume, 1.f, 0.f, true);
        }
    }

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
                SNew(SImage)
                .Image(&BackgroundBrush)
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
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnStartClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("TUTORIAL"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnTutorialClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("SETTINGS"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnSettingsClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("HIGH SCORES"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnHighScoresClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("CREDITS"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnAboutClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]

            // ── DEMO ONLY — remove this entire slot when demo is over ──
            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("DEMO LEVELS"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnDemoLevelsClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]
            // ── END DEMO ──

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 8)
            [
                MakeMenuButton(TEXT("QUIT"),
                    FOnClicked::CreateSP(this, &SMainMenuWidget::OnQuitClicked),
                    FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
            ]
        ];
}

// ══════════════════════════════════════════════════════════════════
// DEMO ONLY — remove this entire function when demo is over
// ══════════════════════════════════════════════════════════════════
TSharedRef<SWidget> SMainMenuWidget::BuildDemoLevelSelectPage()
{
    TSharedRef<SUniformGridPanel> Grid = SNew(SUniformGridPanel)
        .SlotPadding(FMargin(6.f));

    for (int32 i = 1; i <= 20; ++i)
    {
        int32 Row = (i - 1) / 5;
        int32 Col = (i - 1) % 5;

        Grid->AddSlot(Col, Row)
        [
            SNew(SBox)
            .WidthOverride(90.f)
            .HeightOverride(70.f)
            [
                SNew(SButton)
                .ButtonColorAndOpacity(FLinearColor(0.08f, 0.12f, 0.18f, 1.f))
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked_Lambda([this, i]() -> FReply
                {
                    PlayButtonSound();

                    // Write selected level to save file so GameMode
                    // loads the correct level on BeginPlay
                    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
                        UGameplayStatics::LoadGameFromSlot(
                            UOrbitalSaveGame::SaveSlotName, 0));
                    if (!SaveGame)
                        SaveGame = Cast<UOrbitalSaveGame>(
                            UGameplayStatics::CreateSaveGameObject(
                                UOrbitalSaveGame::StaticClass()));

                    if (SaveGame)
                    {
                        SaveGame->SaveCurrentLevel(i);
                        UGameplayStatics::SaveGameToSlot(
                            SaveGame, UOrbitalSaveGame::SaveSlotName, 0);
                    }

                    ULoadingScreen::Show(MyWorld, FName("test"));
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(FString::FromInt(i)))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
                    .ColorAndOpacity(FLinearColor(0.85f, 0.90f, 1.00f, 1.f))
                    .Justification(ETextJustify::Center)
                ]
            ]
        ];
    }

    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(FLinearColor(0.02f, 0.03f, 0.05f, 1.f))
        .Padding(40.f)
        [
            SNew(SVerticalBox)

            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0, 0, 0, 24)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("DEMO — SELECT LEVEL")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 36))
                .ColorAndOpacity(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                .Justification(ETextJustify::Center)
            ]

            // Level grid
            + SVerticalBox::Slot()
            .FillHeight(1.f)
            .HAlign(HAlign_Center)
            [
                Grid
            ]

            // Back button
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .Padding(0, 24, 0, 0)
            [
                SNew(SBox)
                .WidthOverride(120.f)
                .HeightOverride(44.f)
                [
                    SNew(SButton)
                    .ButtonColorAndOpacity(FLinearColor(0.08f, 0.12f, 0.18f, 1.f))
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    .OnClicked_Lambda([this]() -> FReply
                    {
                        NavigateTo(BuildHomePage());
                        return FReply::Handled();
                    })
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("← BACK")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                        .ColorAndOpacity(FLinearColor(0.85f, 0.90f, 1.00f, 1.f))
                    ]
                ]
            ]
        ];
}
// ══════════════════════════════════════════════════════════════════
// END DEMO
// ══════════════════════════════════════════════════════════════════


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
                    PlayButtonSound();
                    NavigateTo(BuildHomePage());
                    return FReply::Handled();
                }))
                .OnHovered(FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("← BACK")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                    .ColorAndOpacity(MM_TextColor)
                ]
            ]
        ];
}

// ─── Settings Page ───────────────────────────────────────────────
TSharedRef<SWidget> SMainMenuWidget::BuildSettingsPage()
{
    // Helper: one labelled slider row
    auto MakeSliderRow = [](const FString& Label, float InitialValue, TFunction<void(float)> OnChanged)
    {
        return SNew(SVerticalBox)

            // Label
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.f, 0.f, 0.f, 8.f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(Label))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 15))
                .ColorAndOpacity(FLinearColor(0.75f, 0.82f, 0.95f, 1.f))
            ]

            // Slider
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SSlider)
                .Value(InitialValue)
                .SliderBarColor(FLinearColor(0.12f, 0.16f, 0.22f, 1.f))
                .SliderHandleColor(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                .OnValueChanged_Lambda(MoveTemp(OnChanged))
            ];
    };

    return SNew(SOverlay)

        // Back button — bottom left
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
                    PlayButtonSound();
                    NavigateTo(BuildHomePage());
                    return FReply::Handled();
                }))
                .OnHovered(FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("← BACK")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                    .ColorAndOpacity(MM_TextColor)
                ]
            ]
        ]

        // Settings panel — top-aligned, horizontally centered
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Top)
        .Padding(0.f, 50.f, 0.f, 0.f)
        [
            SNew(SBox)
            .WidthOverride(500.f)
            [
                SNew(SVerticalBox)

                // Title
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0.f, 0.f, 0.f, 24.f)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("SETTINGS")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
                    .ColorAndOpacity(MM_TitleColor)
                    .Justification(ETextJustify::Center)
                ]

                // Dark settings box — add new settings as new SVerticalBox::Slots below
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(SBorder)
                    .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                    .BorderBackgroundColor(FLinearColor(0.04f, 0.05f, 0.08f, 0.95f))
                    .Padding(FMargin(36.f, 28.f))
                    [
                        SNew(SVerticalBox)

                        // Music Volume
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(0.f, 0.f, 0.f, 28.f)
                        [
                            MakeSliderRow(TEXT("Music Volume"), MusicVolume,
                                [this](float Value)
                                {
                                    MusicVolume = Value;
                                    if (MusicComponent)
                                    {
                                        MusicComponent->SetVolumeMultiplier(Value);
                                        // UE virtualises (stops) audio components at volume 0.
                                        // Restart the component if it stopped while muted.
                                        if (Value > 0.f && !MusicComponent->IsPlaying())
                                            MusicComponent->Play();
                                    }
                                    SaveSettings();
                                })
                        ]

                        // Game Audio Volume
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            MakeSliderRow(TEXT("Game Audio"), GameAudioVolume,
                                [this](float Value)
                                {
                                    GameAudioVolume = Value;
                                    SaveSettings();
                                    USoundMix*   Mix  = LoadObject<USoundMix>  (nullptr, TEXT("/Game/Sounds/SM_GameMix.SM_GameMix"));
                                    USoundClass* Class = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SC_GameAudio.SC_GameAudio"));
                                    if (Mix && Class && MyWorld)
                                    {
                                        UGameplayStatics::PushSoundMixModifier(MyWorld, Mix);
                                        UGameplayStatics::SetSoundMixClassOverride(MyWorld, Mix, Class, Value, 1.f, 0.f, true);
                                    }
                                })
                        ]

                        // ── Add new settings below this line ──
                    ]
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
            .Text(FText::FromString(TEXT("CREDITS")))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
            .ColorAndOpacity(MM_TitleColor)
        ]

        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 20)
        [
            SNew(SBox)
            .WidthOverride(600.f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("Coming soon.")))
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
                    PlayButtonSound();
                    NavigateTo(BuildHomePage());
                    return FReply::Handled();
                }),
                FSimpleDelegate::CreateSP(this, &SMainMenuWidget::PlayHoverSound))
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

    ULoadingScreen::Show(MyWorld, FName("test"));

    return FReply::Handled();
}

FReply SMainMenuWidget::OnTutorialClicked()
{
    PlayButtonSound();
    NavigateTo(BuildTutorialPage());
    return FReply::Handled();
}

FReply SMainMenuWidget::OnSettingsClicked()
{
    PlayButtonSound();
    NavigateTo(BuildSettingsPage());
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

// ── DEMO ONLY — remove this function when demo is over ──
FReply SMainMenuWidget::OnDemoLevelsClicked()
{
    PlayButtonSound();
    NavigateTo(BuildDemoLevelSelectPage());
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

void SMainMenuWidget::PlayHoverSound()
{
    if (ButtonHoverSound && MyWorld)
    {
        UGameplayStatics::PlaySound2D(MyWorld, ButtonHoverSound, 0.5f);
    }
}

void SMainMenuWidget::SaveSettings() const
{
    UOrbitalSettingsSave* Settings = Cast<UOrbitalSettingsSave>(
        UGameplayStatics::LoadGameFromSlot(UOrbitalSettingsSave::SaveSlotName, 0));
    if (!Settings)
        Settings = Cast<UOrbitalSettingsSave>(
            UGameplayStatics::CreateSaveGameObject(UOrbitalSettingsSave::StaticClass()));
    if (Settings)
    {
        Settings->MusicVolume     = MusicVolume;
        Settings->GameAudioVolume = GameAudioVolume;
        UGameplayStatics::SaveGameToSlot(Settings, UOrbitalSettingsSave::SaveSlotName, 0);
    }
}