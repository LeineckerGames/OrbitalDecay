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

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ActorClass = nullptr;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int32 Count = 10;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0f))
    float MinSpacing = 200.0f;

    UPROPERTY(EditAnywhere)
    float ZOffset = 0.0f;

    UPROPERTY(EditAnywhere)
    FVector MeshScale = FVector(1.0f, 1.0f, 1.0f);


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

    UPROPERTY(EditAnywhere, Category = "Border")
    float BorderMargin = 800.0f;       // distance inward from the mesh edge to the playable boundary

    UPROPERTY(EditAnywhere, Category = "Border")
    float BorderWallHeight = 5000.0f;

    UPROPERTY(EditAnywhere, Category = "Border")
    float BorderWallThickness = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Border")
    UMaterialInterface* BorderWallMaterial = nullptr;

    UPROPERTY(EditAnywhere, Category = "Border")
    UStaticMesh* BorderWallMesh = nullptr; // assign Engine cube or your own mesh in BP defaults

    UPROPERTY(EditAnywhere, Category = "Border")
    float CeilingThickness = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Border")
    UStaticMesh* CeilingMesh = nullptr;  // can reuse BorderWallMesh if you want, or assign separately

    UPROPERTY(EditAnywhere, Category = "Border")
    UMaterialInterface* CeilingMaterial = nullptr;

protected:
    virtual void BeginPlay() override;

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
    TArray<AActor*> SpawnedActors;
    TArray<UStaticMeshComponent*> SpawnedObjects;
    float RandomOffset = 0.0f;
    void CreateVertices();
    void CreateTriangles();
    void PlaceObjects(const FObjectPlacementConfig& Config); 
    TArray<UStaticMeshComponent*> BorderWalls; // changed from TArray<UBoxComponent*>
    void CreateBorderWalls();
    void ClearBorderWalls();
    bool IsWithinPlayableBounds(const FVector& LocalVertexPos) const;
    void CreateCeiling();

    void ApplyLevelSettings(int32 Level);
};
