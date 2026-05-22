#include "ReplayRecorder.h"
#include "ALanderPawn.h"
#include "Kismet/GameplayStatics.h"

AReplayRecorder::AReplayRecorder()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AReplayRecorder::BeginPlay()
{
    Super::BeginPlay();

    BufferSize = FMath::Max(1, FMath::RoundToInt(RecordTime * TargetFPS));
    Buffer.SetNum(BufferSize);

    if (!TargetActor)
    {
        TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    }
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

            // Offset replay position so mesh appears at camera position
            // rather than pawn origin, matching what player experienced
            FVector ReplayPos = Frame.Position;
            ALanderPawn* LanderPawn = Cast<ALanderPawn>(TargetActor);
            if (LanderPawn)
            {
                FVector ForwardDir = Frame.Rotation.Vector();
                ReplayPos = Frame.Position - (ForwardDir * 500.f);
            }

            TargetActor->SetActorLocation(ReplayPos);
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
    OnReplayFinished.Broadcast();
}