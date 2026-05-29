#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLevelCompleteWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SLevelCompleteWidget)
        : _OwnerWorld(nullptr)
        , _CompletionTime(0.f)
        , _AccuracyPercent(100.f)
        , _CurrentLevel(1)
    {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
        SLATE_ARGUMENT(float, CompletionTime)
        SLATE_ARGUMENT(float, AccuracyPercent)
        SLATE_ARGUMENT(int32, CurrentLevel)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld          = nullptr;
    float   CompletionTime   = 0.f;
    float   AccuracyPercent  = 100.f;
    int32   CurrentLevel     = 1;
    FString PlayerName       = TEXT("");

    TSharedPtr<SEditableText> NameInputBox;
    TSharedPtr<STextBlock>    SaveStatusText;
    bool bScoreSaved         = false;

    FString FormatTime(float Seconds) const;
    FReply  OnSaveScoreClicked();
    FReply  OnNextLevelClicked();
    FReply  OnRestartClicked();
    FReply  OnMainMenuClicked();
};