#include "SHighScoreWidget.h"
#include "OrbitalSaveGame.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSplitter.h"
#include "Kismet/GameplayStatics.h"

static const FLinearColor HS_Bg      = FLinearColor(0.02f, 0.03f, 0.05f, 1.f);
static const FLinearColor HS_Panel    = FLinearColor(0.05f, 0.07f, 0.10f, 1.f);
static const FLinearColor HS_Border   = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor HS_BtnBg    = FLinearColor(0.08f, 0.12f, 0.18f, 1.f);
static const FLinearColor HS_BtnSel   = FLinearColor(0.10f, 0.40f, 0.15f, 1.f);
static const FLinearColor HS_Title    = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor HS_Text     = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor HS_SubText  = FLinearColor(0.50f, 0.60f, 0.70f, 1.f);
static const FLinearColor HS_Gold     = FLinearColor(1.00f, 0.85f, 0.00f, 1.f);
static const FLinearColor HS_Silver   = FLinearColor(0.75f, 0.75f, 0.75f, 1.f);
static const FLinearColor HS_Bronze   = FLinearColor(0.80f, 0.50f, 0.20f, 1.f);
static const FLinearColor HS_Header   = FLinearColor(0.30f, 0.40f, 0.50f, 1.f);

void SHighScoreWidget::Construct(const FArguments& InArgs)
{
    OnBack = InArgs._OnBack;

    // Build level list on left (20%)
    TSharedRef<SScrollBox> LevelList = SNew(SScrollBox);
    for (int32 i = 1; i <= 20; ++i)
    {
        LevelList->AddSlot()
        [
            SNew(SBox)
            .HeightOverride(44.f)
            [
                SNew(SButton)
                .ButtonColorAndOpacity(i == 1 ? HS_BtnSel : HS_BtnBg)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked_Lambda([this, i]() -> FReply
                {
                    SelectedLevel = i;
                    RefreshScores();
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(FString::Printf(TEXT("Level %d"), i)))
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
                    .ColorAndOpacity(HS_Text)
                ]
            ]
        ];
    }

    ScoreTable = SNew(SVerticalBox);

    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(HS_Bg)
        .Padding(20.f)
        [
            SNew(SVerticalBox)

            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            .Padding(0, 0, 0, 20)
            [
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("HIGH SCORES")))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 42))
                .ColorAndOpacity(HS_Title)
            ]

            // Main content: level list left + scores right
            + SVerticalBox::Slot()
            .FillHeight(1.f)
            [
                SNew(SHorizontalBox)

                // Left: level selector (20%)
                + SHorizontalBox::Slot()
                .FillWidth(0.20f)
                .Padding(0, 0, 10, 0)
                [
                    SNew(SBorder)
                    .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                    .BorderBackgroundColor(HS_Panel)
                    .Padding(4.f)
                    [
                        LevelList
                    ]
                ]

                // Right: score table (80%)
                + SHorizontalBox::Slot()
                .FillWidth(0.80f)
                [
                    SNew(SBorder)
                    .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                    .BorderBackgroundColor(HS_Panel)
                    .Padding(16.f)
                    [
                        ScoreTable.ToSharedRef()
                    ]
                ]
            ]

            // Back button
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .Padding(0, 16, 0, 0)
            [
                SNew(SBox)
                .WidthOverride(120.f)
                .HeightOverride(44.f)
                [
                    SNew(SButton)
                    .ButtonColorAndOpacity(HS_BtnBg)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    .OnClicked(this, &SHighScoreWidget::OnBackClicked)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("← BACK")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                        .ColorAndOpacity(HS_Text)
                    ]
                ]
            ]
        ]
    ];

    RefreshScores();
}

void SHighScoreWidget::RefreshScores()
{
    ScoreTable->ClearChildren();

    // Column headers
    ScoreTable->AddSlot()
    .AutoHeight()
    .Padding(0, 0, 0, 8)
    [
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(HS_Border)
        .Padding(FMargin(8, 6))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot().FillWidth(0.08f)
            [ SNew(STextBlock).Text(FText::FromString(TEXT("#")))
              .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
              .ColorAndOpacity(HS_Header) ]

            + SHorizontalBox::Slot().FillWidth(0.35f)
            [ SNew(STextBlock).Text(FText::FromString(TEXT("NAME")))
              .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
              .ColorAndOpacity(HS_Header) ]

            + SHorizontalBox::Slot().FillWidth(0.30f)
            [ SNew(STextBlock).Text(FText::FromString(TEXT("TIME")))
              .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
              .ColorAndOpacity(HS_Header) ]

            + SHorizontalBox::Slot().FillWidth(0.27f)
            [ SNew(STextBlock).Text(FText::FromString(TEXT("ACCURACY")))
              .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
              .ColorAndOpacity(HS_Header) ]
        ]
    ];

    // Load scores
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(
            UOrbitalSaveGame::SaveSlotName, 0));

    TArray<FScoreEntry> Scores;
    if (SaveGame)
        Scores = SaveGame->GetScores(SelectedLevel);

    if (Scores.Num() == 0)
    {
        ScoreTable->AddSlot()
        .AutoHeight()
        .HAlign(HAlign_Center)
        .Padding(0, 40)
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("No scores yet for this level")))
            .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
            .ColorAndOpacity(HS_SubText)
        ];
        return;
    }

    for (int32 i = 0; i < Scores.Num(); ++i)
    {
        const FScoreEntry& S = Scores[i];

        // Rank color
        FLinearColor RankColor = HS_Text;
        if      (i == 0) RankColor = HS_Gold;
        else if (i == 1) RankColor = HS_Silver;
        else if (i == 2) RankColor = HS_Bronze;

        // Format time as MM:SS
        int32 Mins = FMath::FloorToInt(S.CompletionTimeSeconds / 60.f);
        int32 Secs = FMath::FloorToInt(S.CompletionTimeSeconds) % 60;
        FString TimeStr = FString::Printf(TEXT("%02d:%02d"), Mins, Secs);

        FString AccStr = FString::Printf(TEXT("%.1f%%"), S.AccuracyPercent);

        ScoreTable->AddSlot()
        .AutoHeight()
        .Padding(0, 2)
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(FLinearColor(
                i % 2 == 0 ? 0.06f : 0.04f,
                i % 2 == 0 ? 0.08f : 0.05f,
                i % 2 == 0 ? 0.10f : 0.07f, 1.f))
            .Padding(FMargin(8, 8))
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot().FillWidth(0.08f).VAlign(VAlign_Center)
                [ SNew(STextBlock)
                  .Text(FText::FromString(FString::FromInt(i + 1)))
                  .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
                  .ColorAndOpacity(RankColor) ]

                + SHorizontalBox::Slot().FillWidth(0.35f).VAlign(VAlign_Center)
                [ SNew(STextBlock)
                  .Text(FText::FromString(S.PlayerName))
                  .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                  .ColorAndOpacity(HS_Text) ]

                + SHorizontalBox::Slot().FillWidth(0.30f).VAlign(VAlign_Center)
                [ SNew(STextBlock)
                  .Text(FText::FromString(TimeStr))
                  .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                  .ColorAndOpacity(HS_Text) ]

                + SHorizontalBox::Slot().FillWidth(0.27f).VAlign(VAlign_Center)
                [ SNew(STextBlock)
                  .Text(FText::FromString(AccStr))
                  .Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
                  .ColorAndOpacity(HS_Text) ]
            ]
        ];
    }
}

FReply SHighScoreWidget::OnBackClicked()
{
    OnBack.ExecuteIfBound();
    return FReply::Handled();
}