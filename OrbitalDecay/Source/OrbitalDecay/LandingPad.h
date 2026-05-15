#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "LandingPad.generated.h"

UCLASS()
class ORBITALDECAY_API ALandingPad : public AActor
{
    GENERATED_BODY()

public:
    ALandingPad();

    UPROPERTY(EditAnywhere, Category = "Mesh")
    UStaticMeshComponent* PadMesh;
};
