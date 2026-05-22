// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MissionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ORBITALDECAY_API UMissionCharacter : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Character")
    FString CharacterName;

    UPROPERTY(EditAnywhere, Category = "Character")
    FText MissionDialogue;

    UPROPERTY(EditAnywhere, Category = "Character")
    float BriefingDuration = 5.0f;

    // Portrait or character image for the UI later
    UPROPERTY(EditAnywhere, Category = "Character")
    UTexture2D* CharacterPortrait;
};
