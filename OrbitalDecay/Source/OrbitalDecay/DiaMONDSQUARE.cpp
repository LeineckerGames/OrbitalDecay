// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaMONDSQUARE.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "OrbitalDecayGameMode.h"


ADiaMONDSQUARE::ADiaMONDSQUARE()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());

}

void ADiaMONDSQUARE::BeginPlay()
{
	Super::BeginPlay();

	// --- NEW: pull GlobalLevel and apply tuned values before generating ---
	AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(GetWorld()->GetAuthGameMode());
	int32 CurrentLevel = GM ? GM->GlobalLevel : 1;
	ApplyLevelSettings(CurrentLevel);

	FMath::RandInit(FDateTime::Now().GetTicks());
	RandomOffset = FMath::FRand() * 10000.0f;

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

	ClearBorderWalls(); // NEW: clean up walls before regenerating, same pattern as objects/actors

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);

	CreateBorderWalls(); // NEW

	for (const FObjectPlacementConfig& Config : ObjectLayers)
	{
		PlaceObjects(Config);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("BeginPlay ran"));
}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("BeginPlay ran"));
}


void ADiaMONDSQUARE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiaMONDSQUARE::CreateVertices()
{
	for (int X = 0; X <= XSize; ++X)
	{
		for (int Y = 0; Y <= YSize; ++Y)
		{
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + RandomOffset, Y * NoiseScale + RandomOffset)) * ZMultiplier;
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
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

bool ADiaMONDSQUARE::IsWithinPlayableBounds(const FVector& LocalVertexPos) const
{
	const float MinX = BorderMargin;
	const float MaxX = (XSize * Scale) - BorderMargin;
	const float MinY = BorderMargin;
	const float MaxY = (YSize * Scale) - BorderMargin;

	return LocalVertexPos.X >= MinX && LocalVertexPos.X <= MaxX
		&& LocalVertexPos.Y >= MinY && LocalVertexPos.Y <= MaxY;
}

void ADiaMONDSQUARE::ClearBorderWalls()
{
	for (UBoxComponent* Wall : BorderWalls)
	{
		if (Wall) Wall->DestroyComponent();
	}
	BorderWalls.Reset();
}

void ADiaMONDSQUARE::CreateBorderWalls()
{
	const float MinX = BorderMargin;
	const float MaxX = (XSize * Scale) - BorderMargin;
	const float MinY = BorderMargin;
	const float MaxY = (YSize * Scale) - BorderMargin;
	const float CenterZ = BorderWallHeight * 0.5f;

	auto SpawnWall = [&](FVector LocalCenter, FVector BoxExtent)
	{
		UBoxComponent* Wall = NewObject<UBoxComponent>(this);
		Wall->SetupAttachment(GetRootComponent());
		Wall->RegisterComponent();
		Wall->SetBoxExtent(BoxExtent);
		Wall->SetRelativeLocation(LocalCenter);
		Wall->SetCollisionProfileName(TEXT("BlockAll"));
		Wall->SetVisibility(false); // invisible — terrain still renders past it
		BorderWalls.Add(Wall);
	};

	// North / South walls (run along X, thin along Y)
	SpawnWall(FVector((MinX + MaxX) * 0.5f, MinY, CenterZ),
		FVector((MaxX - MinX) * 0.5f, BorderWallThickness * 0.5f, BorderWallHeight * 0.5f));
	SpawnWall(FVector((MinX + MaxX) * 0.5f, MaxY, CenterZ),
		FVector((MaxX - MinX) * 0.5f, BorderWallThickness * 0.5f, BorderWallHeight * 0.5f));

	// East / West walls (run along Y, thin along X)
	SpawnWall(FVector(MinX, (MinY + MaxY) * 0.5f, CenterZ),
		FVector(BorderWallThickness * 0.5f, (MaxY - MinY) * 0.5f, BorderWallHeight * 0.5f));
	SpawnWall(FVector(MaxX, (MinY + MaxY) * 0.5f, CenterZ),
		FVector(BorderWallThickness * 0.5f, (MaxY - MinY) * 0.5f, BorderWallHeight * 0.5f));
}

void ADiaMONDSQUARE::ApplyLevelSettings(int32 Level)
{
	// Buckets of 5 to match the gravity tiers already used in ALanderPawn
	int32 SteppedLevel = ((Level - 1) / 5) * 5 + 1;

	switch (SteppedLevel)
	{
	case 1:
		ZMultiplier  = 40.0f;
		NoiseScale   = 0.8f;
		BorderMargin = 800.0f;
		break;
	case 6:
		ZMultiplier  = 60.0f;
		NoiseScale   = 1.1f;
		BorderMargin = 900.0f;
		break;
	case 11:
		ZMultiplier  = 85.0f;
		NoiseScale   = 1.4f;
		BorderMargin = 1000.0f;
		break;
	case 16:
		ZMultiplier  = 110.0f;
		NoiseScale   = 1.7f;
		BorderMargin = 1100.0f;
		break;
	default:
		ZMultiplier  = 40.0f;
		NoiseScale   = 0.8f;
		BorderMargin = 800.0f;
		break;
	}
}

void ADiaMONDSQUARE::PlaceObjects(const FObjectPlacementConfig& Config)
{
    if ((!Config.Mesh && !Config.ActorClass) || Vertices.IsEmpty()) return;

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

        FVector WorldPos = GetActorTransform().TransformPosition(Vertices[Idx]);

		 if (!IsWithinPlayableBounds(Vertices[Idx]))
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
            AActor* NewActor = GetWorld()->SpawnActor<AActor>(
                Config.ActorClass,
                SpawnTransform,
                SpawnParams
            );
            if (NewActor)
            {
                SpawnedActors.Add(NewActor);
            }
        }
        // Only use component path if no ActorClass is set
        else if (Config.Mesh)
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
