#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SMissionCompletedWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMissionCompletedWidget)
        : _OwnerWorld(nullptr)
    {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld = nullptr;

    FReply OnMainMenuClicked();
};