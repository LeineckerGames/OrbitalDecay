#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MathGenerator.h"
#include "STutorialOverlay.h"
#include "MyHUD.generated.h"

UCLASS()
class ORBITALDECAY_API AMyHUD : public AHUD
{
    GENERATED_BODY()
public:
    AMyHUD();

    FSlateBrush CockpitBrush;
    TSharedPtr<class SCockpitWidget> MyCockpitWidget;
    TSharedPtr<class SCrashScreen>   MyCrashScreen;
    TSharedPtr<class SPauseWidget>   MyPauseWidget;

    FString CurrentQuestionText;
    int32   CurrentCorrectAnswer = 0;
    bool    bQuestionActive = false;
    bool    bIsPaused = false;

    UPROPERTY()
    UMathGenerator* MathEngine;

    FSlateFontInfo QuestionFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Settings")
    int32 CurrentLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Settings")
    FString QuestionType = TEXT("a");

    UPROPERTY(EditAnywhere, Category = "Math Settings")
    float FuelRewardAmount = 100.0f;

    UFUNCTION(Exec)
    void SetQuestion(FString Type, int32 Level);

    void GenerateNewQuestion(FString Type, int32 Level);
    void ShowSuccessScreen();
    void ShowFailureScreen();
    void HideResultScreen();
    void ShowCrashScreen();
    void ShowPauseScreen();
    void HidePauseScreen();

    TSharedPtr<class SLevelCompleteWidget> MyLevelCompleteWidget;
    int32 TotalQuestionsAnswered = 0;
    int32 TotalQuestionsCorrect  = 0;

    float GetAccuracyPercent() const
    {
        if (TotalQuestionsAnswered == 0) return 100.f;
        return (float)TotalQuestionsCorrect /
            (float)TotalQuestionsAnswered * 100.f;
    }

    void ShowLevelCompleteScreen(float CompletionTime, int32 Level);

    // Tutorial system
    void ShowTutorialStep(const FString& Title, const FString& Body);
    void CheckTutorialTriggers();
    bool bTutorialStep1Shown = false; // Welcome + HUD overview
    bool bTutorialStep2Shown = false; // Math input
    bool bTutorialStep3Shown = false; // First correct answer
    bool bTutorialStep4Shown = false; // Low fuel
    bool bTutorialStep5Shown = false; // Near landing pad
    bool bTutorialStep6Shown = false; // First pad landed
    bool bTutorialOverlayActive = false;

    TSharedPtr<STutorialOverlay> MyTutorialOverlay;

    // Cached once in BeginPlay — avoids LoadGameFromSlot every frame
    bool bCachedIsTutorialRun = false;

protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

private:
    bool bShowSuccess = false;
    bool bShowFailure = false;
};