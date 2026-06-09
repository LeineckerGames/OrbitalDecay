#include "MyHUD.h"
#include "SCockpitWidget.h"
#include "Widgets/SWeakWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "OrbitalDecayGameMode.h"
#include "ALanderPawn.h"
#include "Engine/Canvas.h"
#include "ReplayRecorder.h"
#include "SCrashScreen.h"
#include "SPauseWidget.h"
#include "SLevelCompleteWidget.h"
#include "EngineUtils.h"
#include "Framework/Application/SlateApplication.h"

AMyHUD::AMyHUD()
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> CockpitAsset(TEXT("/Game/Textures/Cockpit_interior"));
    if (CockpitAsset.Succeeded())
    {
        CockpitBrush.SetResourceObject(CockpitAsset.Object);
        CockpitBrush.ImageSize = FVector2D(1920, 1080);
    }
    QuestionFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 48);
}

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();
    MathEngine = NewObject<UMathGenerator>(this);
    CurrentQuestionText = TEXT("");
    CurrentCorrectAnswer = -1;
    bQuestionActive = false;

    if (GEngine && GEngine->GameViewport)
    {
        MyCockpitWidget = SNew(SCockpitWidget).OwnerHUD(this);
        GEngine->GameViewport->AddViewportWidgetContent(
            SNew(SWeakWidget).PossiblyNullContent(MyCockpitWidget.ToSharedRef())
        );

        // Bind to replay finished so we show crash screen after replay ends
        for (TActorIterator<AReplayRecorder> It(GetWorld()); It; ++It)
        {
            It->OnReplayFinished.AddLambda([this]()
            {
                ShowCrashScreen();
            });
            break;
        }

        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MyCockpitWidget);
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }

        FSlateApplication::Get().SetKeyboardFocus(MyCockpitWidget);
    }
}

void AMyHUD::GenerateNewQuestion(FString Type, int32 Level)
{
    if (MathEngine)
    {
        QuestionType = Type;
        FMathQuestion NewQ = MathEngine->GenerateQuestion(Type, Level);
        CurrentQuestionText = NewQ.QuestionText;
        CurrentCorrectAnswer = NewQ.CorrectAnswer;
    }
}

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();

    // Check replay state every frame and show/hide HUD accordingly
    AReplayRecorder* Recorder = nullptr;
    for (TActorIterator<AReplayRecorder> It(GetWorld()); It; ++It)
    {
        Recorder = *It;
        break;
    }

    if (Recorder)
    {
        if (Recorder->IsReplaying())
        {
            if (MyCockpitWidget.IsValid() &&
                MyCockpitWidget->GetVisibility() != EVisibility::Hidden)
            {
                MyCockpitWidget->SetVisibility(EVisibility::Hidden);
            }
        }
    }

    // Draw success/failure overlay text in the center of the screen
    // Canvas->SizeX/SizeY gives screen dimensions for positioning
    if (bShowSuccess)
    {
        DrawText(TEXT("LANDED SUCCESSFULLY"), FColor::Green, Canvas->SizeX * 0.35f, Canvas->SizeY * 0.45f, nullptr, 2.5f);
    }
    else if (bShowFailure)
    {
        DrawText(TEXT("CRASHED"), FColor::Red, Canvas->SizeX * 0.45f, Canvas->SizeY * 0.45f, nullptr, 2.5f);
    }
}

void AMyHUD::SetQuestion(FString Type, int32 Level)
{
    AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->GlobalLevel = Level;

    QuestionType = Type;
    CurrentLevel = Level;
    GenerateNewQuestion(QuestionType, CurrentLevel);
}

void AMyHUD::ShowSuccessScreen()
{
    // Show success overlay
    bShowSuccess = true;
    bShowFailure = false;

    // Disable cockpit widget input so player can't fire thrusters during replay
    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(false);
}

void AMyHUD::ShowFailureScreen()
{
    // Show failure overlay
    bShowSuccess = false;
    bShowFailure = true;

    // Disable cockpit widget input so player can't fire thrusters during replay
    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(false);
}

void AMyHUD::HideResultScreen()
{
    // Clear both overlays once replay begins
    bShowSuccess = false;
    bShowFailure = false;

    // Re-enable cockpit input for the next round (Sprint 5)
    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(true);
}

void AMyHUD::ShowCrashScreen()
{
    if (!GEngine || !GEngine->GameViewport) return;

    // Show cursor so player can click Retry
    APlayerController* PC = GetOwningPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
    }

    MyCrashScreen = SNew(SCrashScreen).OwnerWorld(GetWorld());
    GEngine->GameViewport->AddViewportWidgetContent(
        MyCrashScreen.ToSharedRef(), 10); // zorder 10 = on top of cockpit
}

void AMyHUD::ShowPauseScreen()
{
    if (bIsPaused) return;
    bIsPaused = true;

    // Pause the game — freezes all ticking, physics, timers
    APlayerController* PC = GetOwningPlayerController();
    if (PC) PC->SetPause(true);

    // Disable cockpit input so player can't answer questions while paused
    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(false);

    if (GEngine && GEngine->GameViewport)
    {
        MyPauseWidget = SNew(SPauseWidget)
            .OwnerWorld(GetWorld())
            .OnResume_Lambda([this]()
            {
                HidePauseScreen();
            });

        GEngine->GameViewport->AddViewportWidgetContent(
            MyPauseWidget.ToSharedRef(), 5); // zorder 5, below crash screen
    }
}

void AMyHUD::HidePauseScreen()
{
    if (!bIsPaused) return;
    bIsPaused = false;

    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(true);

    if (GEngine && GEngine->GameViewport && MyPauseWidget.IsValid())
    {
        GEngine->GameViewport->RemoveViewportWidgetContent(
            MyPauseWidget.ToSharedRef());
        MyPauseWidget.Reset();
    }

    // Restore focus and input mode to cockpit widget
    APlayerController* PC = GetOwningPlayerController();
    if (PC)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(MyCockpitWidget);
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }

    FSlateApplication::Get().SetKeyboardFocus(MyCockpitWidget);
}

void AMyHUD::ShowLevelCompleteScreen(float CompletionTime, int32 Level)
{
    if (!GEngine || !GEngine->GameViewport) return;

    // Pause so nothing moves while screen is shown
    APlayerController* PC = GetOwningPlayerController();
    if (PC)
    {
        PC->SetPause(true);
        PC->bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
    }

    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(false);

    MyLevelCompleteWidget = SNew(SLevelCompleteWidget)
        .OwnerWorld(GetWorld())
        .CompletionTime(CompletionTime)
        .AccuracyPercent(GetAccuracyPercent())
        .CurrentLevel(Level);

    GEngine->GameViewport->AddViewportWidgetContent(
        MyLevelCompleteWidget.ToSharedRef(), 10);
}