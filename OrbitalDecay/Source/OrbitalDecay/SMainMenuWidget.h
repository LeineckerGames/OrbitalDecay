#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

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
    TSharedRef<SWidget> BuildHighScoresPage();
    TSharedRef<SWidget> BuildTutorialPage();
    TSharedRef<SWidget> BuildSettingsPage();
    TSharedRef<SWidget> BuildAboutPage();

    USoundWave*     ButtonClickSound = nullptr;
    USoundWave*     ButtonHoverSound = nullptr;
    USoundWave*     MainMenuMusic    = nullptr;
    UAudioComponent* MusicComponent  = nullptr;

    float MusicVolume     = 0.7f;
    float GameAudioVolume = 1.0f;

    void PlayButtonSound();
    void PlayHoverSound();
    void SaveSettings() const;



    // Navigation
    void NavigateTo(TSharedRef<SWidget> NewPage);

    // Button handlers
    FReply OnStartClicked();
    FReply OnTutorialClicked();
    FReply OnSettingsClicked();
    FReply OnHighScoresClicked();
    FReply OnAboutClicked();
    FReply OnQuitClicked();
};