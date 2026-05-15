#include "LandingPad.h"

ALandingPad::ALandingPad()
{
    PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
    RootComponent = PadMesh;

    PadMesh->SetCollisionProfileName(TEXT("BlockAll"));

    Tags.Add(FName("LandingPad"));
}

