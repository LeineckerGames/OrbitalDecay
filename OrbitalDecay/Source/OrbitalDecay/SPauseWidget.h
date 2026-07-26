#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Sound/SoundWave.h"

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
    UWorld*           MyWorld          = nullptr;
    FOnResumeDelegate OnResume;
    USoundWave*       ButtonClickSound = nullptr;
    USoundWave*       ButtonHoverSound = nullptr;

    void PlayButtonSound();
    void PlayHoverSound();

    FReply OnResumeClicked();
    FReply OnRestartClicked();
    FReply OnMainMenuClicked();
};