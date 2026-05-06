#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplayRecorder.generated.h"

USTRUCT(BlueprintType)
struct FTransformFrame
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Position = FVector::ZeroVector;

    UPROPERTY()
    FRotator Rotation = FRotator::ZeroRotator;
};

UCLASS()
class ORBITALDECAY_API AReplayRecorder : public AActor
{
    GENERATED_BODY()

public:
    AReplayRecorder();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
    AActor* TargetActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
    float RecordTime = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay")
    float TargetFPS = 60.0f;

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StartReplay();

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopReplay();

    UFUNCTION(BlueprintPure, Category = "Replay")
    bool IsReplaying() const { return bReplaying; }

private:
    TArray<FTransformFrame> Buffer;
    int32 BufferSize = 0;
    int32 FrameIndex = 0;
    int32 ReplayIndex = 0;
    int32 ReplayFramesPlayed = 0;
    bool bReplaying = false;
    float TimeAccumulator = 0.f;
};
