#include "ReplayRecorder.h"

AReplayRecorder::AReplayRecorder()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplaying = false;
    FrameIndex = 0;
    ReplayIndex = 0;
}

void AReplayRecorder::BeginPlay()
{
    Super::BeginPlay();

    BufferSize = FMath::RoundToInt(RecordTime * TargetFPS);
    Buffer.SetNum(BufferSize);
}

void AReplayRecorder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bReplaying)
    {
        // Replay mode
        const FTransformFrame& Frame = Buffer[ReplayIndex];

        SetActorLocation(Frame.Position);
        SetActorRotation(Frame.Rotation);

        ReplayIndex = (ReplayIndex + 1) % BufferSize;
        return;
    }

    // Recording mode
    FTransformFrame& Frame = Buffer[FrameIndex];

    Frame.Position = GetActorLocation();
    Frame.Rotation = GetActorRotation();

    FrameIndex = (FrameIndex + 1) % BufferSize;
}

void AReplayRecorder::StartReplay()
{
    bReplaying = true;
    ReplayIndex = FrameIndex; // start from oldest frame
}

void AReplayRecorder::StopReplay()
{
    bReplaying = false;
}
