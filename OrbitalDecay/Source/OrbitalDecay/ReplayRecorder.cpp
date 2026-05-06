#include "ReplayRecorder.h"

AReplayRecorder::AReplayRecorder()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AReplayRecorder::BeginPlay()
{
    Super::BeginPlay();

    BufferSize = FMath::Max(1, FMath::RoundToInt(RecordTime * TargetFPS));
    Buffer.SetNum(BufferSize);
}

void AReplayRecorder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetActor)
    {
        return;
    }

    const float FrameInterval = 1.f / TargetFPS;
    TimeAccumulator += DeltaTime;

    while (TimeAccumulator >= FrameInterval)
    {
        TimeAccumulator -= FrameInterval;

        if (bReplaying)
        {
            const FTransformFrame& Frame = Buffer[ReplayIndex];
            TargetActor->SetActorLocation(Frame.Position);
            TargetActor->SetActorRotation(Frame.Rotation);

            ReplayIndex = (ReplayIndex + 1) % BufferSize;
            ReplayFramesPlayed++;

            if (ReplayFramesPlayed >= BufferSize)
            {
                StopReplay();
            }
        }
        else
        {
            FTransformFrame& Frame = Buffer[FrameIndex];
            Frame.Position = TargetActor->GetActorLocation();
            Frame.Rotation = TargetActor->GetActorRotation();

            FrameIndex = (FrameIndex + 1) % BufferSize;
        }
    }
}

void AReplayRecorder::StartReplay()
{
    if (bReplaying)
    {
        return;
    }

    bReplaying = true;
    ReplayFramesPlayed = 0;
    ReplayIndex = FrameIndex;
}

void AReplayRecorder::StopReplay()
{
    bReplaying = false;
}
