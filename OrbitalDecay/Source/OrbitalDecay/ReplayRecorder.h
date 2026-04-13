#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplayRecorder.generated.h"

USTRUCT()
struct FTransformFrame
{
    GENERATED_BODY()

    FVector Position;
    FRotator Rotation;
};

UCLASS()
class YOURGAME_API AReplayRecorder : public AActor
{
    GENERATED_BODY()

public:
    AReplayRecorder();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UFUNCTION(BlueprintCallable)
    void StartReplay();

    UFUNCTION(BlueprintCallable)
    void StopReplay();

private:
    TArray<FTransformFrame> Buffer;

    int32 BufferSize;
    int32 FrameIndex;

    float RecordTime = 10.0f;
    float TargetFPS = 60.0f;

    bool bReplaying;
    int32 ReplayIndex;
};
