#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(FOnBackDelegate);

class SLevelSelectWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SLevelSelectWidget)
        : _OwnerWorld(nullptr)
    {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
        SLATE_EVENT(FOnBackDelegate, OnBack)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld*          MyWorld      = nullptr;
    FOnBackDelegate  OnBack;
    int32            SelectedLevel = -1;

    TSharedPtr<STextBlock> SelectedLevelText;
    TSharedPtr<SButton>    PlayButton;

    TSharedRef<SWidget> BuildLevelGrid();
    void OnLevelButtonClicked(int32 Level);
    FReply OnPlayClicked();
    FReply OnBackClicked();
};