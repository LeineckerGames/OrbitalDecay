// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// The following include is required for Unreal Header Tool (UHT) to generate code for UCLASS/USTRUCT/UENUM macros.
// Do NOT remove or rename this line. The file "MathGenerator.generated.h" is auto-generated during the build process.
#include "MathGenerator.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMathQuestion
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString QuestionText;

    UPROPERTY(BlueprintReadOnly)
    int32 CorrectAnswer;
};

UCLASS()
class ORBITALDECAY_API UMathGenerator : public UObject
{
    GENERATED_BODY()

public:
    FMathQuestion GenerateQuestion(FString Type, int32 Level);
};