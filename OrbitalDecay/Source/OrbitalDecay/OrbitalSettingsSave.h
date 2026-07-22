#pragma once
#include "GameFramework/SaveGame.h"
#include "OrbitalSettingsSave.generated.h"

UCLASS()
class ORBITALDECAY_API UOrbitalSettingsSave : public USaveGame
{
    GENERATED_BODY()
public:
    static const FString SaveSlotName;

    UPROPERTY()
    float MusicVolume = 0.7f;

    UPROPERTY()
    float GameAudioVolume = 1.0f;

    UPROPERTY()
    bool bLegacyMode = false;
};
