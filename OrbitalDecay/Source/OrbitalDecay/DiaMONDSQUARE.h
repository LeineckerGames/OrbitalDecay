// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DiaMONDSQUARE.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FObjectPlacementConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    UStaticMesh* Mesh = nullptr;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int32 Count = 10;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0f))
    float MinSpacing = 200.0f;

    UPROPERTY(EditAnywhere)
    float ZOffset = 0.0f;
};

UCLASS()
class ORBITALDECAY_API ADiaMONDSQUARE : public AActor
{
    GENERATED_BODY()

public:
    ADiaMONDSQUARE();

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int XSize = 0;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int YSize = 0;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    float ZMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    float NoiseScale = 1.0f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float Scale = 0;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float UVScale = 0;

    UPROPERTY(EditAnywhere)               
        TArray<FObjectPlacementConfig> ObjectLayers;

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* Material;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UProceduralMeshComponent* ProceduralMesh;
    TArray<FVector> Vertices;
    TArray<int> Triangles;
    TArray<FVector2D> UV0;
    TArray<FVector> Normals;
    TArray<struct FProcMeshTangent> Tangents;
    TArray<UStaticMeshComponent*> SpawnedObjects;

    void CreateVertices();
    void CreateTriangles();
    void PlaceObjects(const FObjectPlacementConfig& Config); 
};