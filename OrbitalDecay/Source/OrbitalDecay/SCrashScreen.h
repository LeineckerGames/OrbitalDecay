#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class ORBITALDECAY_API SCrashScreen : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCrashScreen)
        : _OwnerWorld(nullptr)
        , _CurrentLevel(1)
    {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
        SLATE_ARGUMENT(int32, CurrentLevel)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld      = nullptr;
    int32   CurrentLevel = 1;

    FReply OnRetryClicked();
    FReply OnMainMenuClicked();
};