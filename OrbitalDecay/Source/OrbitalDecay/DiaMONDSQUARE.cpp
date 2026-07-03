// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaMONDSQUARE.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "OrbitalDecayGameMode.h"
#include "ALanderPawn.h"


ADiaMONDSQUARE::ADiaMONDSQUARE()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());

}

void ADiaMONDSQUARE::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Use the preview level while editing.
    ApplyLevelSettings(PreviewLevel);

    // Deterministic terrain based on the seed.
    RandomOffset = Seed * 137.5f;

    Vertices.Reset();
    Triangles.Reset();
    UV0.Reset();
    Normals.Reset();
    Tangents.Reset();

    CreateVertices();
    CreateTriangles();

    ProceduralMesh->ClearAllMeshSections();

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
        Vertices,
        Triangles,
        UV0,
        Normals,
        Tangents
    );

    ProceduralMesh->CreateMeshSection(
        0,
        Vertices,
        Triangles,
        Normals,
        UV0,
        TArray<FColor>(),
        Tangents,
        true
    );

    UpdateTerrainMaterial(PreviewLevel);
}
void ADiaMONDSQUARE::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE::BeginPlay START"));

	Super::BeginPlay();

	AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(GetWorld()->GetAuthGameMode());
	int32 CurrentLevel = GM ? GM->GlobalLevel : PreviewLevel;

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: GM valid=%s, GlobalLevel=%d"), GM ? TEXT("true") : TEXT("false"), CurrentLevel);
	
	ApplyLevelSettings(CurrentLevel);

	UE_LOG(LogTemp, Warning, TEXT("Current Level = %d"), CurrentLevel);

	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE: randomising seed"));
	FMath::RandInit(FDateTime::Now().GetTicks());
	RandomOffset = FMath::FRand() * 10000.0f;

	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE: clearing old data"));
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	for (UStaticMeshComponent* Obj : SpawnedObjects)
	{
		if (Obj) Obj->DestroyComponent();
	}
	SpawnedObjects.Reset();

	for (AActor* Actor : SpawnedActors)
	{
		if (Actor) Actor->Destroy();
	}
	SpawnedActors.Reset();

	ClearBorderWalls();

	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE: building mesh"));
	ProceduralMesh->ClearAllMeshSections();
	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		Vertices,
		Triangles,
		UV0,
		Normals,
		Tangents
	);

	ProceduralMesh->CreateMeshSection(
		0,
		Vertices,
		Triangles,
		Normals,
		UV0,
		TArray<FColor>(),
		Tangents,
		true
	);

	UpdateTerrainMaterial(CurrentLevel);

	CreateBorderWalls();
	CreateCeiling();

	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE: placing objects (%d layers)"), ObjectLayers.Num());
	for (int32 i = 0; i < ObjectLayers.Num(); ++i)
	{
		const FObjectPlacementConfig& Config = ObjectLayers[i];

		UE_LOG(LogTemp, Warning,
			TEXT("DiaMONDSQUARE: PlaceObjects layer %d — ActorClass=%s Mesh=%s Count=%d"),
			i,
			Config.ActorClass ? *Config.ActorClass->GetName() : TEXT("null"),
			Config.Mesh ? *Config.Mesh->GetName() : TEXT("null"),
			Config.Count);

		PlaceObjects(Config);

		UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE: layer %d done"), i);
	}

	UE_LOG(LogTemp, Warning, TEXT("DiaMONDSQUARE::BeginPlay DONE"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("BeginPlay ran"));
}

void ADiaMONDSQUARE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiaMONDSQUARE::CreateVertices()
{
	// Center the mesh around the actor's origin so it doesn't spawn offset to one corner.
	float HalfX = (XSize * Scale) * 0.5f;
	float HalfY = (YSize * Scale) * 0.5f;

	for (int X = 0; X <= XSize; ++X)
	{
		for (int Y = 0; Y <= YSize; ++Y)
		{
			float LocalX = X * Scale - HalfX;
			float LocalY = Y * Scale - HalfY;

			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + RandomOffset, Y * NoiseScale + RandomOffset)) * ZMultiplier;

			// Spherical curvature: terrain drops away toward the horizon like a planet surface.
			if (bEnablePlanetCurvature && PlanetRadius > 0.0f)
			{
				// Use square-max distance (Chebyshev) so the flat playable zone is square,
				// not circular. NormX/Y are 0 at centre, 1 at the mesh edge midpoint.
				float NormX      = (HalfX > 0.0f) ? FMath::Abs(LocalX) / HalfX : 0.0f;
				float NormY      = (HalfY > 0.0f) ? FMath::Abs(LocalY) / HalfY : 0.0f;
				float SquareDist = FMath::Max(NormX, NormY); // 0 = centre, 1 = edge

				// Smoothstep from the falloff threshold to the edge.
				// Inside the threshold = 0 (flat); at the edge = 1 (full curvature).
				float T              = FMath::Clamp((SquareDist - CurvatureEdgeFalloff) / (1.0f - CurvatureEdgeFalloff), 0.0f, 1.0f);
				float CurvatureBlend = T * T * (3.0f - 2.0f * T);

				float DistSq  = LocalX * LocalX + LocalY * LocalY;
				float RSq     = PlanetRadius * PlanetRadius;
				float CurveDrop = (DistSq < RSq)
					? PlanetRadius - FMath::Sqrt(RSq - DistSq)
					: PlanetRadius;

				Z -= CurveDrop * CurvatureBlend;
			}

			Vertices.Add(FVector(LocalX, LocalY, Z));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void ADiaMONDSQUARE::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < XSize; ++X) {

		for (int Y = 0; Y < YSize; ++Y) {
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			++Vertex;

		}
		++Vertex;
	}
}

bool ADiaMONDSQUARE::IsWithinPlayableBounds(const FVector& LocalVertexPos, float ExtraPadding) const
{
	// ExtraPadding shrinks the zone further inward per-layer so large objects
	// can't clip into the border walls even when their mesh extends past the pivot.
	const float HalfX = (XSize * Scale) * 0.5f;
	const float HalfY = (YSize * Scale) * 0.5f;
	const float TotalMargin = BorderMargin + ExtraPadding;

	const float MinX = -HalfX + TotalMargin;
	const float MaxX =  HalfX - TotalMargin;
	const float MinY = -HalfY + TotalMargin;
	const float MaxY =  HalfY - TotalMargin;

	return LocalVertexPos.X >= MinX && LocalVertexPos.X <= MaxX
		&& LocalVertexPos.Y >= MinY && LocalVertexPos.Y <= MaxY;
}

void ADiaMONDSQUARE::ClearBorderWalls()
{
	for (UStaticMeshComponent* Wall : BorderWalls)   // change from UBoxComponent*
	{
		if (Wall) Wall->DestroyComponent();
	}
	BorderWalls.Reset();
}

void ADiaMONDSQUARE::CreateBorderWalls()
{
	const float HalfX = (XSize * Scale) * 0.5f;
	const float HalfY = (YSize * Scale) * 0.5f;

	const float MinX = -HalfX + BorderMargin;
	const float MaxX = HalfX - BorderMargin;
	const float MinY = -HalfY + BorderMargin;
	const float MaxY = HalfY - BorderMargin;
	const float CenterZ = BorderWallHeight * 0.5f;

	auto SpawnWall = [&](FVector LocalCenter, FVector BoxExtent)
		{
			UStaticMeshComponent* Wall = NewObject<UStaticMeshComponent>(this);
			Wall->SetupAttachment(GetRootComponent());
			Wall->RegisterComponent();

			if (BorderWallMesh)
			{
				Wall->SetStaticMesh(BorderWallMesh);
			}

			if (BorderWallMaterial)
			{
				Wall->SetMaterial(0, BorderWallMaterial);
			}

			Wall->SetRelativeLocation(LocalCenter);
			Wall->SetRelativeScale3D(BoxExtent / 50.0f);
			Wall->SetCollisionProfileName(TEXT("BlockAll"));
			Wall->SetCastShadow(false);
			Wall->OnComponentHit.AddDynamic(this, &ADiaMONDSQUARE::OnBorderHit);

			BorderWalls.Add(Wall);
		};

	SpawnWall(FVector((MinX + MaxX) * 0.5f, MinY, CenterZ),
		FVector((MaxX - MinX) * 0.5f, BorderWallThickness * 0.5f, BorderWallHeight * 0.5f));
	SpawnWall(FVector((MinX + MaxX) * 0.5f, MaxY, CenterZ),
		FVector((MaxX - MinX) * 0.5f, BorderWallThickness * 0.5f, BorderWallHeight * 0.5f));
	SpawnWall(FVector(MinX, (MinY + MaxY) * 0.5f, CenterZ),
		FVector(BorderWallThickness * 0.5f, (MaxY - MinY) * 0.5f, BorderWallHeight * 0.5f));
	SpawnWall(FVector(MaxX, (MinY + MaxY) * 0.5f, CenterZ),
		FVector(BorderWallThickness * 0.5f, (MaxY - MinY) * 0.5f, BorderWallHeight * 0.5f));
}

void ADiaMONDSQUARE::CreateCeiling()
{
	const float HalfX = (XSize * Scale) * 0.5f;
	const float HalfY = (YSize * Scale) * 0.5f;

	const float MinX = -HalfX + BorderMargin;
	const float MaxX = HalfX - BorderMargin;
	const float MinY = -HalfY + BorderMargin;
	const float MaxY = HalfY - BorderMargin;

	UStaticMeshComponent* Ceiling = NewObject<UStaticMeshComponent>(this);
	Ceiling->SetupAttachment(GetRootComponent());
	Ceiling->RegisterComponent();

	if (CeilingMesh)
	{
		Ceiling->SetStaticMesh(CeilingMesh);
	}

	if (CeilingMaterial)
	{
		Ceiling->SetMaterial(0, CeilingMaterial);
	}

	const FVector LocalCenter((MinX + MaxX) * 0.5f, (MinY + MaxY) * 0.5f, BorderWallHeight);
	const FVector BoxExtent((MaxX - MinX) * 0.5f, (MaxY - MinY) * 0.5f, CeilingThickness * 0.5f);

	Ceiling->SetRelativeLocation(LocalCenter);
	Ceiling->SetRelativeScale3D(BoxExtent / 50.0f);
	Ceiling->SetCollisionProfileName(TEXT("BlockAll"));
	Ceiling->SetCastShadow(false);
	Ceiling->OnComponentHit.AddDynamic(this, &ADiaMONDSQUARE::OnBorderHit);

	BorderWalls.Add(Ceiling);
}
void ADiaMONDSQUARE::OnBorderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bBorderTriggersCrash) return;

	ALanderPawn* Lander = Cast<ALanderPawn>(OtherActor);
	if (Lander)
	{
		Lander->TriggerCrash();
	}
}

void ADiaMONDSQUARE::ApplyLevelSettings(int32 Level)
{
	// Buckets of 5 to match the gravity tiers already used in ALanderPawn
	int32 SteppedLevel = ((Level - 1) / 5) * 5 + 1;

	switch (SteppedLevel)
	{
	case 1:
		XSize                = 160;
		YSize                = 160;
		Scale                = 1000.0f;
		ZMultiplier          = 1200.0f;
		NoiseScale           = 0.2f;
		BorderMargin         = 72000.0f; // walls at ±8000 UU from centre (10% playable of 160000 map)
		bEnablePlanetCurvature = true;
		PlanetRadius         = 500000.0f;
		CurvatureEdgeFalloff = 0.7f;
		break;
	case 6:
		XSize                = 160;
		YSize                = 160;
		Scale                = 1000.0f;
		ZMultiplier          = 2500.0f; // taller than level 1
		NoiseScale           = 0.25f;   // broad hills, not spikes
		BorderMargin         = 72000.0f;
		bEnablePlanetCurvature = true;
		PlanetRadius         = 500000.0f;
		CurvatureEdgeFalloff = 0.7f;
		break;
	case 11:
		XSize                = 160;
		YSize                = 160;
		Scale                = 1000.0f;
		ZMultiplier          = 4000.0f; // dramatic mountain-scale height
		NoiseScale           = 0.22f;   // slightly broader features than level 6
		BorderMargin         = 72000.0f;
		bEnablePlanetCurvature = true;
		PlanetRadius         = 500000.0f;
		CurvatureEdgeFalloff = 0.7f;
		break;
	case 16:
		XSize                = 160;
		YSize                = 160;
		Scale                = 1000.0f;
		ZMultiplier          = 6000.0f; // very tall peaks
		NoiseScale           = 0.2f;    // same frequency as level 1 — broad and imposing
		BorderMargin         = 72000.0f;
		bEnablePlanetCurvature = true;
		PlanetRadius         = 500000.0f;
		CurvatureEdgeFalloff = 0.7f;
		break;
	default:
		XSize                = 160;
		YSize                = 160;
		Scale                = 1000.0f;
		ZMultiplier          = 1200.0f;
		NoiseScale           = 0.2f;
		BorderMargin         = 72000.0f;
		bEnablePlanetCurvature = true;
		PlanetRadius         = 500000.0f;
		CurvatureEdgeFalloff = 0.7f;
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
		FString::Printf(TEXT("NoiseScale=%f ZMultiplier=%f Level=%d"), NoiseScale, ZMultiplier, Level));
}

void ADiaMONDSQUARE::UpdateTerrainMaterial(int32 Level)
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateTerrainMaterial: Level=%d, MaterialsNum=%d"), Level, TerrainMaterials.Num());

    if (TerrainMaterials.Num() == 0)
        return;

    int32 MaterialIndex = (Level - 1) / 5;
    MaterialIndex = FMath::Clamp(MaterialIndex, 0, TerrainMaterials.Num() - 1);

    UE_LOG(LogTemp, Warning, TEXT("UpdateTerrainMaterial: MaterialIndex=%d"), MaterialIndex);

    ProceduralMesh->SetMaterial(0, TerrainMaterials[MaterialIndex]);
}

void ADiaMONDSQUARE::PlaceObjects(const FObjectPlacementConfig& Config)
{
    // Guard: need a world, valid vertices, and at least one thing to place.
    if (!GetWorld() || Vertices.IsEmpty()) return;

    // Use IsValid() instead of bare null checks — a stale Blueprint reference can be
    // non-null yet point to garbage, and calling ANY method on it (including GetName()
    // or IsChildOf) will crash before we even reach SpawnActor.
    const bool bHasValidClass = IsValid(Config.ActorClass);
    const bool bHasValidMesh  = IsValid(Config.Mesh);
    if (!bHasValidMesh && !bHasValidClass) return;

    // Guard: ActorClass must be a real AActor subclass, not a stale/null CDO reference.
    // A bad class here causes a fatal CastChecked failure inside SpawnActor.
    if (bHasValidClass && !Config.ActorClass->IsChildOf(AActor::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaceObjects: ActorClass '%s' is not an AActor subclass — skipping."),
            *Config.ActorClass->GetName());
        return;
    }

    TArray<FVector> PlacedPositions;

    TArray<int32> Indices;
    for (int32 i = 0; i < Vertices.Num(); ++i) Indices.Add(i);
    for (int32 i = Indices.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        Indices.Swap(i, j);
    }

    int32 Placed = 0;

    for (int32 Idx : Indices)
    {
        if (Placed >= Config.Count) break;

        // Reject vertices outside the spawn radius (local X/Y, ignoring terrain height).
        if (Config.MaxSpawnRadius > 0.0f)
        {
            float Dist2D = FMath::Sqrt(Vertices[Idx].X * Vertices[Idx].X + Vertices[Idx].Y * Vertices[Idx].Y);
            if (Dist2D > Config.MaxSpawnRadius) continue;
        }

        // Find the highest terrain Z within SurfaceSampleRadius grid cells.
        // This prevents large objects (e.g. landing pads) from embedding into
        // sloped terrain where a nearby vertex is higher than the spawn vertex.
        float LocalZ = Vertices[Idx].Z;
        if (Config.SurfaceSampleRadius > 0)
        {
            const int32 Vx = Idx / (YSize + 1);
            const int32 Vy = Idx % (YSize + 1);
            const int32 R  = Config.SurfaceSampleRadius;
            for (int32 Dx = -R; Dx <= R; ++Dx)
            {
                for (int32 Dy = -R; Dy <= R; ++Dy)
                {
                    const int32 Nx = Vx + Dx;
                    const int32 Ny = Vy + Dy;
                    if (Nx >= 0 && Nx <= XSize && Ny >= 0 && Ny <= YSize)
                    {
                        const int32 NIdx = Nx * (YSize + 1) + Ny;
                        if (NIdx < Vertices.Num())
                            LocalZ = FMath::Max(LocalZ, Vertices[NIdx].Z);
                    }
                }
            }
        }

        FVector WorldPos = GetActorTransform().TransformPosition(
            FVector(Vertices[Idx].X, Vertices[Idx].Y, LocalZ));

        if (!IsWithinPlayableBounds(Vertices[Idx], Config.SpawnBorderPadding))
        {
            continue;
        }

        bool bTooClose = false;
        for (const FVector& Existing : PlacedPositions)
        {
            if (FVector::Dist(WorldPos, Existing) < Config.MinSpacing)
            {
                bTooClose = true;
                break;
            }
        }
        if (bTooClose) continue;

        FVector SpawnLocation = WorldPos + FVector(0.0f, 0.0f, Config.ZOffset);

        // Actor path takes priority — cleaner, scale is reliable
        if (Config.ActorClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation, Config.MeshScale);
            AActor* NewActor = GetWorld()->SpawnActor(Config.ActorClass, &SpawnTransform, SpawnParams);
            if (NewActor)
            {
                NewActor->SetActorScale3D(Config.MeshScale);
                SpawnedActors.Add(NewActor);
            }
        }
        // Only use component path if no ActorClass is set
        else if (bHasValidMesh)
        {
            UStaticMeshComponent* NewObj = NewObject<UStaticMeshComponent>(this);
            NewObj->SetStaticMesh(Config.Mesh);
            NewObj->SetupAttachment(GetRootComponent());
            NewObj->RegisterComponent();
            NewObj->SetWorldLocation(SpawnLocation);
            NewObj->SetWorldScale3D(Config.MeshScale);
            SpawnedObjects.Add(NewObj);
        }

        PlacedPositions.Add(WorldPos);
        ++Placed;
    }
}
