// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaMONDSQUARE.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"


ADiaMONDSQUARE::ADiaMONDSQUARE()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());

}

void ADiaMONDSQUARE::BeginPlay()
{
	Super::BeginPlay();

	FMath::RandInit(FDateTime::Now().GetTicks());
	RandomOffset = FMath::FRand() * 10000.0f; // store to member, don't calc Z here

	Vertices.Reset();
	
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

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);

	for (const FObjectPlacementConfig& Config : ObjectLayers)
	{
		PlaceObjects(Config);
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

		// --- existing mesh component spawn (unchanged) ---
		if (Config.Mesh)
		{
			UStaticMeshComponent* NewObj = NewObject<UStaticMeshComponent>(this);
			NewObj->SetStaticMesh(Config.Mesh);
			NewObj->SetupAttachment(GetRootComponent());
			NewObj->RegisterComponent();
			NewObj->SetWorldLocation(SpawnLocation);
			SpawnedObjects.Add(NewObj);
		}

		// --- NEW: actor spawn ---
		if (Config.ActorClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			AActor* NewActor = GetWorld()->SpawnActor<AActor>(
				Config.ActorClass,
				SpawnLocation,
				FRotator::ZeroRotator,
				SpawnParams
			);
			if (NewActor)
			{
				SpawnedActors.Add(NewActor);
			}
		}

		PlacedPositions.Add(WorldPos);
		++Placed;
	}
}
