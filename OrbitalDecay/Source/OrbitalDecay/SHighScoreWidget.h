#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(FOnHighScoreBackDelegate);

class SHighScoreWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SHighScoreWidget) {}
        SLATE_EVENT(FOnHighScoreBackDelegate, OnBack)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FOnHighScoreBackDelegate OnBack;
    int32 SelectedLevel = 1;

    TSharedPtr<SVerticalBox> ScoreTable;

    TSharedPtr<SScrollBox> LevelListBox;
    void RebuildLevelList();

    void RefreshScores();
    FReply OnBackClicked();
};