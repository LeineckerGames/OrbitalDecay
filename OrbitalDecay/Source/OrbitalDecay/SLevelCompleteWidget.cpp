#include "SLevelCompleteWidget.h"
#include "OrbitalSaveGame.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitalSaveGame.h"
#include "OrbitalDecayGameMode.h"

static const FLinearColor LC_Bg       = FLinearColor(0.02f, 0.03f, 0.05f, 0.93f);
static const FLinearColor LC_Panel    = FLinearColor(0.05f, 0.08f, 0.06f, 1.f);
static const FLinearColor LC_Border   = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor LC_BtnBg    = FLinearColor(0.08f, 0.12f, 0.18f, 1.f);
static const FLinearColor LC_BtnGreen = FLinearColor(0.08f, 0.28f, 0.10f, 1.f);
static const FLinearColor LC_BtnRed   = FLinearColor(0.25f, 0.05f, 0.05f, 1.f);
static const FLinearColor LC_Title    = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor LC_Text     = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor LC_SubText  = FLinearColor(0.50f, 0.60f, 0.70f, 1.f);
static const FLinearColor LC_Input    = FLinearColor(0.03f, 0.08f, 0.04f, 1.f);

FString SLevelCompleteWidget::FormatTime(float Seconds) const
{
    int32 Mins = FMath::FloorToInt(Seconds / 60.f);
    int32 Secs = FMath::FloorToInt(Seconds) % 60;
    int32 Ms   = FMath::FloorToInt(FMath::Fmod(Seconds, 1.f) * 100.f);
    return FString::Printf(TEXT("%02d:%02d.%02d"), Mins, Secs, Ms);
}

void SLevelCompleteWidget::Construct(const FArguments& InArgs)
{
    MyWorld         = InArgs._OwnerWorld;
    CompletionTime  = InArgs._CompletionTime;
    AccuracyPercent = InArgs._AccuracyPercent;
    CurrentLevel    = InArgs._CurrentLevel;

    // Check if score qualifies for top 10
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(
            UOrbitalSaveGame::SaveSlotName, 0));

    bool bQualifies = true;
    if (SaveGame)
    {
        TArray<FScoreEntry> Scores = SaveGame->GetScores(CurrentLevel);
        if (Scores.Num() >= 10)
            bQualifies = CompletionTime < Scores.Last().CompletionTimeSeconds;
    }

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
            .BorderBackgroundColor(LC_Bg)
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
                .BorderBackgroundColor(LC_Panel)
                .Padding(32.f)
                [
                    SNew(SVerticalBox)

                    // LEVEL COMPLETE title
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 8)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("LEVEL COMPLETE")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 36))
                        .ColorAndOpacity(LC_Title)
                        .Justification(ETextJustify::Center)
                    ]

                    // Level number
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 20)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(
                            FString::Printf(TEXT("Level %d"), CurrentLevel)))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                        .ColorAndOpacity(LC_SubText)
                        .Justification(ETextJustify::Center)
                    ]

                    // Stats box
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0, 0, 0, 20)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor(0.03f,0.05f,0.04f,1.f))
                        .Padding(16.f)
                        [
                            SNew(SVerticalBox)

                            // Time row
                            + SVerticalBox::Slot()
                            .AutoHeight()
                            .Padding(0, 4)
                            [
                                SNew(SHorizontalBox)

                                + SHorizontalBox::Slot()
                                .FillWidth(1.f)
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("COMPLETION TIME")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
                                    .ColorAndOpacity(LC_SubText)
                                ]

                                + SHorizontalBox::Slot()
                                .AutoWidth()
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(FormatTime(CompletionTime)))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                                    .ColorAndOpacity(LC_Title)
                                ]
                            ]

                            // Accuracy row
                            + SVerticalBox::Slot()
                            .AutoHeight()
                            .Padding(0, 4)
                            [
                                SNew(SHorizontalBox)

                                + SHorizontalBox::Slot()
                                .FillWidth(1.f)
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("ANSWER ACCURACY")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
                                    .ColorAndOpacity(LC_SubText)
                                ]

                                + SHorizontalBox::Slot()
                                .AutoWidth()
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(
                                        FString::Printf(TEXT("%.1f%%"), AccuracyPercent)))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                                    .ColorAndOpacity(LC_Title)
                                ]
                            ]
                        ]
                    ]

                    // Name entry section
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0, 0, 0, 8)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(bQualifies ?
                            TEXT("Enter your name for the leaderboard:") :
                            TEXT("Enter your name to save your score:")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 13))
                        .ColorAndOpacity(LC_SubText)
                    ]

                    // Name input box
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0, 0, 0, 6)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(LC_Input)
                        .Padding(FMargin(8, 6))
                        [
                            SAssignNew(NameInputBox, SEditableText)
                            .HintText(FText::FromString(TEXT("Your name...")))
                            .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                            .ColorAndOpacity(LC_Text)
                        ]
                    ]

                    // Save score button
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Fill)
                    .Padding(0, 0, 0, 6)
                    [
                        SNew(SBox)
                        .HeightOverride(44.f)
                        [
                            SNew(SButton)
                            .ButtonColorAndOpacity(LC_BtnGreen)
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            .OnClicked(this, &SLevelCompleteWidget::OnSaveScoreClicked)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(TEXT("SAVE SCORE")))
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                                .ColorAndOpacity(LC_Text)
                            ]
                        ]
                    ]

                    // Save status message
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .HAlign(HAlign_Center)
                    .Padding(0, 0, 0, 16)
                    [
                        SAssignNew(SaveStatusText, STextBlock)
                        .Text(FText::GetEmpty())
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 13))
                        .ColorAndOpacity(LC_Title)
                        .Justification(ETextJustify::Center)
                    ]

                    // Divider
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0, 0, 0, 16)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(LC_Border)
                        .Padding(FMargin(0, 1))
                        [ SNew(SSpacer) ]
                    ]

                    // Navigation buttons
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    [
                        SNew(SHorizontalBox)

                        // Next Level (hidden on level 20)
                        + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        .Padding(0, 0, 4, 0)
                        [
                            SNew(SBox)
                            .HeightOverride(48.f)
                            .Visibility(CurrentLevel >= 20 ?
                                EVisibility::Collapsed : EVisibility::Visible)
                            [
                                SNew(SButton)
                                .ButtonColorAndOpacity(LC_BtnGreen)
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Center)
                                .OnClicked(this,
                                    &SLevelCompleteWidget::OnNextLevelClicked)
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("NEXT LEVEL")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
                                    .ColorAndOpacity(LC_Text)
                                ]
                            ]
                        ]

                        

                        // Main Menu
                        + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        .Padding(4, 0, 0, 0)
                        [
                            SNew(SBox)
                            .HeightOverride(48.f)
                            [
                                SNew(SButton)
                                .ButtonColorAndOpacity(LC_BtnRed)
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Center)
                                .OnClicked(this,
                                    &SLevelCompleteWidget::OnMainMenuClicked)
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("MAIN MENU")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
                                    .ColorAndOpacity(LC_Text)
                                ]
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];
}

FReply SLevelCompleteWidget::OnSaveScoreClicked()
{
    if (bScoreSaved) return FReply::Handled();
    if (!NameInputBox.IsValid()) return FReply::Handled();

    FString Name = NameInputBox->GetText().ToString().TrimStartAndEnd();
    if (Name.IsEmpty())
    {
        SaveStatusText->SetText(
            FText::FromString(TEXT("Please enter a name first.")));
        return FReply::Handled();
    }

    // Load or create save game
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(
            UOrbitalSaveGame::SaveSlotName, 0));
    if (!SaveGame)
        SaveGame = Cast<UOrbitalSaveGame>(
            UGameplayStatics::CreateSaveGameObject(
                UOrbitalSaveGame::StaticClass()));

    if (SaveGame)
    {
        FScoreEntry Entry;
        Entry.PlayerName             = Name;
        Entry.CompletionTimeSeconds  = CompletionTime;
        Entry.AccuracyPercent        = AccuracyPercent;

        // Date string
        FDateTime Now = FDateTime::Now();
        Entry.DateString = FString::Printf(TEXT("%04d-%02d-%02d"),
            Now.GetYear(), Now.GetMonth(), Now.GetDay());

        SaveGame->AddScore(CurrentLevel, Entry);
        bScoreSaved = true;

        // Check if they made the top 10
        TArray<FScoreEntry> Scores = SaveGame->GetScores(CurrentLevel);
        bool bMadeIt = false;
        for (const FScoreEntry& S : Scores)
        {
            if (S.PlayerName == Name &&
                FMath::IsNearlyEqual(S.CompletionTimeSeconds,
                    CompletionTime, 0.01f))
            {
                bMadeIt = true;
                break;
            }
        }

        SaveStatusText->SetText(FText::FromString(
            bMadeIt ? TEXT("Score saved! Check the leaderboard.")
                    : TEXT("Score saved! (Not in top 10)")));
    }

    return FReply::Handled();
}

FReply SLevelCompleteWidget::OnNextLevelClicked()
{
    if (GEngine && GEngine->GameViewport)
        GEngine->GameViewport->RemoveAllViewportWidgets();

    if (MyWorld)
    {
        // Advance to the next level NOW — this is the only
        // place GlobalLevel should ever be incremented+saved
        AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(
            MyWorld->GetAuthGameMode());
        if (GM)
        {
            GM->TriggerLevelComplete();
        }

        UGameplayStatics::OpenLevel(MyWorld,
            FName(*UGameplayStatics::GetCurrentLevelName(MyWorld)));
    }
    return FReply::Handled();
}



FReply SLevelCompleteWidget::OnMainMenuClicked()
{
    if (GEngine && GEngine->GameViewport)
        GEngine->GameViewport->RemoveAllViewportWidgets();

    if (MyWorld)
        UGameplayStatics::OpenLevel(MyWorld, FName("MainMenu"));

    return FReply::Handled();
}