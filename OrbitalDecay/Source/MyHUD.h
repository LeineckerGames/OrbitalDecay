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
    FString QuestionType = TEXT("a"); // "a" = add, "s" = sub, "m" = mul, "d" = div

    UPROPERTY(EditAnywhere, Category = "Math Settings")
    float FuelRewardAmount = 100.0f;

    UFUNCTION(Exec)
    void SetQuestion(FString Type, int32 Level);

    // Call this whenever you want a new question
    void GenerateNewQuestion(FString Type, int32 Level);

protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;
};