#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SMainMenuWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMainMenuWidget) : _OwnerWorld(nullptr) {}
        SLATE_ARGUMENT(UWorld*, OwnerWorld)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    UWorld* MyWorld = nullptr;

    // Current page shown in the content area
    TSharedPtr<SWidget> CurrentPage;
    TSharedPtr<SBox>    ContentArea;

    // Page builders
    TSharedRef<SWidget> BuildHomePage();
    TSharedRef<SWidget> BuildLevelSelectPage();
    TSharedRef<SWidget> BuildHighScoresPage();
    TSharedRef<SWidget> BuildTutorialPage();
    TSharedRef<SWidget> BuildAboutPage();

    // Navigation
    void NavigateTo(TSharedRef<SWidget> NewPage);

    // Button handlers
    FReply OnStartClicked();
    FReply OnTutorialClicked();
    FReply OnHighScoresClicked();
    FReply OnAboutClicked();
    FReply OnQuitClicked();
};