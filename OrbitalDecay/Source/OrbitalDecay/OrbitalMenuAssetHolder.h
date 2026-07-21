#pragma once
#include "GameFramework/Actor.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Engine/Texture2D.h"
#include "OrbitalMenuAssetHolder.generated.h"

/**
 * Place one of these in the MainMenu level and assign every asset in the
 * Details panel.  The UPROPERTY hard-references cause the cooker to include
 * all referenced assets in the package, fixing missing audio/images in builds.
 */
UCLASS()
class ORBITALDECAY_API AOrbitalMenuAssetHolder : public AActor
{
    GENERATED_BODY()

public:
    // ── Sounds ──────────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    USoundWave* ButtonClickSound = nullptr;

    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    USoundWave* ButtonHoverSound = nullptr;

    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    USoundWave* MainMenuMusic = nullptr;

    // ── Audio mix / class used by the Game Audio slider ─────────────
    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    USoundMix* GameSoundMix = nullptr;

    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    USoundClass* GameAudioClass = nullptr;

    // ── Textures ────────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    UTexture2D* BackgroundTexture = nullptr;

    UPROPERTY(EditAnywhere, Category = "Menu Assets")
    UTexture2D* TutorialTexture = nullptr;
};
