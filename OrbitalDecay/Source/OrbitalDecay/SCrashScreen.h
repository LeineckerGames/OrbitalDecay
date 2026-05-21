#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class ORBITALDECAY_API SCrashScreen : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCrashScreen) : _OwnerWorld(nullptr) {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld = nullptr;
    FReply OnRetryClicked();
};