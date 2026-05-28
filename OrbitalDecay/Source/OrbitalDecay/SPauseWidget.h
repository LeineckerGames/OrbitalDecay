#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(FOnResumeDelegate);

class SPauseWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SPauseWidget)
        : _OwnerWorld(nullptr)
    {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
        SLATE_EVENT(FOnResumeDelegate, OnResume)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld*          MyWorld  = nullptr;
    FOnResumeDelegate OnResume;

    FReply OnResumeClicked();
    FReply OnRestartClicked();
    FReply OnMainMenuClicked();
};