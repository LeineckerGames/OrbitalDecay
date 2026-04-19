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

void ADiaMONDSQUARE::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();

	for (UStaticMeshComponent* Obj : SpawnedObjects)
	{
		if (Obj) Obj->DestroyComponent();
	}
	SpawnedObjects.Reset();

	CreateVertices();
	CreateTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);

	// Run placement for every layer
	for (const FObjectPlacementConfig& Config : ObjectLayers)
	{
		PlaceObjects(Config);
	}
}

void ADiaMONDSQUARE::BeginPlay()
{
	Super::BeginPlay();
	
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
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale+ 0.1, Y * NoiseScale+ 0.1)) * ZMultiplier;
			//GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			UV0.Add(FVector2D(X*UVScale,Y*UVScale));

			//debug
			//DrawDebugSphere(GetWorld(), FVector(X * Scale, Y * Scale, 0), 25.0f, 16, FColor::Red, true, -1.0f, 0U, 0.0f);
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
	if (!Config.Mesh || Vertices.IsEmpty()) return;

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

		UStaticMeshComponent* NewObj = NewObject<UStaticMeshComponent>(this);
		NewObj->SetStaticMesh(Config.Mesh);
		NewObj->SetupAttachment(GetRootComponent());
		NewObj->RegisterComponent();
		NewObj->SetWorldLocation(WorldPos + FVector(0.0f, 0.0f, Config.ZOffset));

		SpawnedObjects.Add(NewObj);
		PlacedPositions.Add(WorldPos);
		++Placed;
	}
}