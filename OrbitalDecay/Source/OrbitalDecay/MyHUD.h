#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MathGenerator.h"
#include "MyHUD.generated.h"

UCLASS()
class ORBITALDECAY_API AMyHUD : public AHUD
{
    GENERATED_BODY()
public:
    AMyHUD();

    FSlateBrush CockpitBrush;
    TSharedPtr<class SCockpitWidget> MyCockpitWidget;

    FString CurrentQuestionText;
    int32 CurrentCorrectAnswer = 0;
    bool bQuestionActive = false;

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

    // Shows a full-screen overlay and disables cockpit input
    void ShowSuccessScreen();
    void ShowFailureScreen();
    void HideResultScreen();

protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

private:
    // Tracks which overlay to draw in DrawHUD
    bool bShowSuccess = false;
    bool bShowFailure = false;
};