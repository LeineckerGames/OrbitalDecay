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

    // Maximum distance from the centre of the map this layer may spawn.
    // 0 = no restriction (use the full terrain).
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0f))
    float MaxSpawnRadius = 0.0f;

    // Extra inset from the border walls for this layer's spawn zone.
    // Set this to at least half the object's footprint so no part of
    // the mesh can clip into a boundary wall.
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0f))
    float SpawnBorderPadding = 0.0f;

    // How many grid cells to sample around the spawn vertex when finding the
    // surface height. The highest Z found is used as the base, preventing the
    // object from embedding into sloped terrain. 0 = use the vertex Z only.
    // Set to 2-3 for large objects like landing pads; leave at 0 for small rocks.
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int32 SurfaceSampleRadius = 0;
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

    // Enable to bend the terrain surface as if it wraps around a sphere,
    // creating a visible curved horizon. Smaller radius = more dramatic curve.
    UPROPERTY(EditAnywhere, Category = "Planet")
    bool bEnablePlanetCurvature = false;

    // Radius of the planet in Unreal units. Try 50000–500000.
    // Smaller = tighter curve visible at shorter distances.
    UPROPERTY(EditAnywhere, Category = "Planet", Meta = (EditCondition = "bEnablePlanetCurvature", ClampMin = 1000.0))
    float PlanetRadius = 200000.0f;

    // Fraction of the half-extent that stays flat before curvature begins (0–1).
    // 0 = curvature starts at the centre; 0.7 = inner 70% is flat playable area,
    // outer 30% curves down to the horizon. Match this to your border edge.
    UPROPERTY(EditAnywhere, Category = "Planet", Meta = (EditCondition = "bEnablePlanetCurvature", ClampMin = 0.0, ClampMax = 1.0))
    float CurvatureEdgeFalloff = 0.7f;

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

    UPROPERTY(EditAnywhere, Category = "Border")
    bool bBorderTriggersCrash = false;   // toggle on/off, checked live at hit time

    // Seed used for editor preview and (optionally) runtime generation.
    // Change this to get a different terrain shape without pressing Play.
    UPROPERTY(EditAnywhere, Category = "Generation")
    int32 Seed = 42;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
	
    UPROPERTY(EditAnywhere, Category = "Terrain")
    TArray<UMaterialInterface*> TerrainMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int32 PreviewLevel = 6;

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
    bool IsWithinPlayableBounds(const FVector& LocalVertexPos, float ExtraPadding = 0.0f) const;
    void CreateCeiling();
    void UpdateTerrainMaterial(int32 Level);
    UFUNCTION()
    void OnBorderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


    void ApplyLevelSettings(int32 Level);
};