#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(FOnTutorialStepDismissed);

class ORBITALDECAY_API STutorialOverlay : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STutorialOverlay)
        : _OwnerWorld(nullptr)
        , _TitleText(TEXT(""))
        , _BodyText(TEXT(""))
        {
        }
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
        SLATE_ARGUMENT(FString, TitleText)
        SLATE_ARGUMENT(FString, BodyText)
        SLATE_EVENT(FOnTutorialStepDismissed, OnDismissed)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld = nullptr;
    FOnTutorialStepDismissed OnDismissed;

    FReply OnGotItClicked();
};