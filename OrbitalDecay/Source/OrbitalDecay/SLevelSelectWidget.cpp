#include "SLevelSelectWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitalSaveGame.h"
#include "Sound/SoundWave.h"

static const FLinearColor LS_Bg        = FLinearColor(0.02f, 0.03f, 0.05f, 1.f);
static const FLinearColor LS_Border    = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor LS_BtnBg     = FLinearColor(0.08f, 0.12f, 0.18f, 1.f);
static const FLinearColor LS_BtnSel    = FLinearColor(0.10f, 0.50f, 0.20f, 1.f);
static const FLinearColor LS_PlayBg    = FLinearColor(0.10f, 0.40f, 0.15f, 1.f);
static const FLinearColor LS_Title     = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor LS_Text      = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor LS_SubText   = FLinearColor(0.50f, 0.60f, 0.70f, 1.f);
static const FLinearColor LS_TestBg    = FLinearColor(0.40f, 0.20f, 0.05f, 1.f);

void SLevelSelectWidget::Construct(const FArguments& InArgs)
{
    MyWorld = InArgs._OwnerWorld;
    OnBack  = InArgs._OnBack;

    ButtonClickSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sounds/342200__christopherderp__videogame-menu-button-click.342200__christopherderp__videogame-menu-button-click"));

    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(LS_Bg)
        .Padding(40.f)
        [
            SNew(SVerticalBox)

            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0, 0, 0, 30)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("SELECT LEVEL")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
                .ColorAndOpacity(LS_Title)
            ]

            // Level grid
            + SVerticalBox::Slot()
            .FillHeight(1.f)
            [
                BuildLevelGrid()
            ]

            // Selection display + play button
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0, 20, 0, 0)
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(0, 0, 20, 0)
                [
                    SAssignNew(SelectedLevelText, STextBlock)
                    .Text(FText::FromString(TEXT("No level selected")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 18))
                    .ColorAndOpacity(LS_SubText)
                ]

                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SBox)
                    .WidthOverride(160.f)
                    .HeightOverride(50.f)
                    [
                        SAssignNew(PlayButton, SButton)
                        .ButtonColorAndOpacity(LS_PlayBg)
                        .HAlign(HAlign_Center)
                        .VAlign(VAlign_Center)
                        .IsEnabled(false)
                        .OnClicked(this, &SLevelSelectWidget::OnPlayClicked)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("PLAY")))
                            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
                            .ColorAndOpacity(LS_Text)
                        ]
                    ]
                ]
            ]

            // Back button
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .Padding(0, 20, 0, 0)
            [
                SNew(SBox)
                .WidthOverride(120.f)
                .HeightOverride(44.f)
                [
                    SNew(SButton)
                    .ButtonColorAndOpacity(LS_BtnBg)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    .OnClicked(this, &SLevelSelectWidget::OnBackClicked)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("← BACK")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                        .ColorAndOpacity(LS_Text)
                    ]
                ]
            ]
        ]
    ];
}

TSharedRef<SWidget> SLevelSelectWidget::BuildLevelGrid()
{
    TSharedRef<SUniformGridPanel> Grid = SNew(SUniformGridPanel)
        .SlotPadding(FMargin(6.f));

    // Levels 1-20 in a 5x4 grid
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
                .ButtonColorAndOpacity(LS_BtnBg)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked_Lambda([this, i]() -> FReply
                {
                    OnLevelButtonClicked(i);
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(FString::FromInt(i)))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
                    .ColorAndOpacity(LS_Text)
                    .Justification(ETextJustify::Center)
                ]
            ]
        ];
    }

    // Test level button in its own row below
    Grid->AddSlot(0, 4)
    [
        SNew(SBox)
        .WidthOverride(90.f)
        .HeightOverride(70.f)
        [
            SNew(SButton)
            .ButtonColorAndOpacity(LS_TestBg)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .OnClicked_Lambda([this]() -> FReply
            {
                PlayButtonSound();
                if (MyWorld)
                    UGameplayStatics::OpenLevel(MyWorld, FName("test"));
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("TEST")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                .ColorAndOpacity(LS_Text)
                .Justification(ETextJustify::Center)
            ]
        ]
    ];

    return Grid;
}

void SLevelSelectWidget::OnLevelButtonClicked(int32 Level)
{
    PlayButtonSound();
    SelectedLevel = Level;
    SelectedLevelText->SetText(
        FText::FromString(FString::Printf(TEXT("Level %d selected"), Level)));
    PlayButton->SetEnabled(true);
}

FReply SLevelSelectWidget::OnPlayClicked()
{
    PlayButtonSound();

    if (MyWorld && SelectedLevel > 0)
    {
        /*// Levels 1-20 will map to level files when created
        // For now just log — wire to actual level files when ready
        UE_LOG(LogTemp, Warning, TEXT("Loading Level %d"), SelectedLevel);

        // When level files exist, use:
        // FString LevelName = FString::Printf(TEXT("Level_%d"), SelectedLevel);
        // UGameplayStatics::OpenLevel(MyWorld, FName(*LevelName));*/

        //Save the selected level so BeginPlay loads it correctly
        UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
            UGameplayStatics::LoadGameFromSlot(
                UOrbitalSaveGame::SaveSlotName, 0));
        if (!SaveGame)
            SaveGame = Cast<UOrbitalSaveGame>(
                UGameplayStatics::CreateSaveGameObject(
                    UOrbitalSaveGame::StaticClass()));
        if (SaveGame) SaveGame->SaveCurrentLevel(SelectedLevel);

        FTimerHandle LoadLevelTimer;
        MyWorld->GetTimerManager().SetTimer(LoadLevelTimer, [this]()
            {
                UGameplayStatics::OpenLevel(MyWorld, FName("test"));
            }, 0.3f, false);

    }
    return FReply::Handled();
}

FReply SLevelSelectWidget::OnBackClicked()
{
    PlayButtonSound();
    OnBack.ExecuteIfBound();
    return FReply::Handled();
}

void SLevelSelectWidget::PlayButtonSound()
{
    if (ButtonClickSound && MyWorld)
        UGameplayStatics::PlaySound2D(MyWorld, ButtonClickSound);
}