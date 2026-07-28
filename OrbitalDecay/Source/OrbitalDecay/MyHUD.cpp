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
#include "OrbitalSettingsSave.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "OrbitalSaveGame.h"

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

    // Clear any widgets that persisted from the previous world
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveAllViewportWidgets();
    }

    MathEngine = NewObject<UMathGenerator>(this);
    CurrentQuestionText = TEXT("");
    CurrentCorrectAnswer = -1;
    bQuestionActive = false;

    // Apply saved game audio volume so the setting carries over from the main menu
    UOrbitalSettingsSave* Settings = Cast<UOrbitalSettingsSave>(
        UGameplayStatics::LoadGameFromSlot(UOrbitalSettingsSave::SaveSlotName, 0));
    if (Settings)
    {
        USoundMix*   Mix   = LoadObject<USoundMix>  (nullptr, TEXT("/Game/Sounds/SM_GameMix.SM_GameMix"));
        USoundClass* Class = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sounds/SC_GameAudio.SC_GameAudio"));
        if (Mix && Class)
        {
            UGameplayStatics::PushSoundMixModifier(GetWorld(), Mix);
            UGameplayStatics::SetSoundMixClassOverride(GetWorld(), Mix, Class, Settings->GameAudioVolume, 1.f, 0.f, true);
        }
    }

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
    CheckTutorialTriggers();

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

    // Read current level from game mode so crash screen can display it
    int32 LevelNum = 1;
    AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(
        GetWorld()->GetAuthGameMode());
    if (GM) LevelNum = GM->GlobalLevel;

    MyCrashScreen = SNew(SCrashScreen)
        .OwnerWorld(GetWorld())
        .CurrentLevel(LevelNum);
        
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

void AMyHUD::ShowTutorialStep(const FString& Title, const FString& Body)
{
    if (!GEngine || !GEngine->GameViewport) return;
    if (bTutorialOverlayActive) return;

    bTutorialOverlayActive = true;

    // Pause the game while the overlay is shown
    APlayerController* PC = GetOwningPlayerController();
    if (PC) PC->SetPause(true);

    // Disable cockpit input
    if (MyCockpitWidget.IsValid())
        MyCockpitWidget->SetInputEnabled(false);

    MyTutorialOverlay = SNew(STutorialOverlay)
        .OwnerWorld(GetWorld())
        .TitleText(Title)
        .BodyText(Body)
        .OnDismissed_Lambda([this]()
            {
                bTutorialOverlayActive = false;

                // Re-enable cockpit input
                if (MyCockpitWidget.IsValid())
                {
                    MyCockpitWidget->SetInputEnabled(true);
                    FSlateApplication::Get().SetKeyboardFocus(MyCockpitWidget);
                }
                    
            });

    GEngine->GameViewport->AddViewportWidgetContent(
        MyTutorialOverlay.ToSharedRef(), 20);
}

void AMyHUD::CheckTutorialTriggers()
{
    // Only run during a tutorial run
    UOrbitalSaveGame* SaveGame = Cast<UOrbitalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(
            UOrbitalSaveGame::SaveSlotName, 0));
    if (!SaveGame || !SaveGame->IsTutorialRun()) return;

    // Don't check while an overlay is already showing
    if (bTutorialOverlayActive) return;

    ALanderPawn* Pawn = Cast<ALanderPawn>(GetOwningPawn());
    AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(
        GetWorld()->GetAuthGameMode());
    if (!Pawn || !GM) return;

    // Step 1 — Welcome, fires when briefing ends and game starts
    if (!bTutorialStep1Shown)
    {
        bTutorialStep1Shown = true;
        ShowTutorialStep(
            TEXT("WELCOME TO ORBITAL DECAY"),
            TEXT("Your ship's computer is failing. You are the backup.\n\n"
                "Watch your cockpit carefully:\n"
                "• Fuel gauge — top left\n"
                "• Math problem display — center\n"
                "• Minimap — top right\n"
                "• Altitude and vertical speed — bottom right\n\n"
                "Gravity is always pulling you down. Don't wait too long.")
        );
        return;
    }

    // Step 2 — Math input, fires after step 1 is shown
    if (!bTutorialStep2Shown && bTutorialStep1Shown)
    {
        bTutorialStep2Shown = true;
        ShowTutorialStep(
            TEXT("HOW TO CONTROL YOUR SHIP"),
            TEXT("Math IS your controls. Press a key to get a problem:\n\n"
                "• W — Multiplication → Thrust upward\n"
                "• A — Addition → Rotate left\n"
                "• S — Subtraction → Rotate right\n"
                "• D — Division → Add fuel\n\n"
                "Type your answer using the number keys or keypad, then press Enter.\n"
                "Get it right and your ship responds. Get it wrong — nothing happens.")
        );
        return;
    }

    // Step 3 — Operation result, fires after first correct answer
    if (!bTutorialStep3Shown && TotalQuestionsCorrect >= 1)
    {
        bTutorialStep3Shown = true;
        ShowTutorialStep(
            TEXT("NICE WORK"),
            TEXT("You answered correctly and your ship responded.\n\n"
                "Every correct answer fires a thruster for a short burst.\n"
                "Keep solving problems to stay in control.\n\n"
                "Remember — gravity never stops. Keep thrusting upward (W) "
                "to slow your descent.")
        );
        return;
    }

    // Step 4 — Low fuel, fires when fuel drops below 50%
    if (!bTutorialStep4Shown && Pawn->Fuel < (Pawn->MaxFuel * 0.5f))
    {
        bTutorialStep4Shown = true;
        ShowTutorialStep(
            TEXT("FUEL WARNING"),
            TEXT("Your fuel is getting low.\n\n"
                "Press D to get a Division problem.\n"
                "Solve it correctly to add fuel to your tank.\n\n"
                "If you run out of fuel completely, only gravity acts on your ship "
                "and you will crash. Refuel early.")
        );
        return;
    }

    // Step 5 — Near landing pad, fires when a pad is close
    // Uses PadsLanded as a proxy — minimap ring shows when near
    // Step 5 — Near landing pad
    if (!bTutorialStep5Shown && bTutorialStep1Shown && bTutorialStep2Shown &&
        bTutorialStep3Shown && Pawn->PadsLanded == 0 &&
        Pawn->CurrentAltitude < 3000.f && Pawn->CurrentAltitude > 0.f &&
        Pawn->LevelTimeSeconds > 10.f)
    {
        bTutorialStep5Shown = true;
        ShowTutorialStep(
            TEXT("LANDING PAD NEARBY"),
            TEXT("Check your minimap.\n\n"
                "• Yellow ring — a landing pad is nearby\n"
                "• Red ring — you are directly above a pad\n\n"
                "When you see a red ring, slow your descent using W (thrust up).\n"
                "Land too fast and you will crash even on the pad.\n\n"
                "Watch your vertical speed indicator — keep it close to zero.")
        );
        return;
    }

    // Step 6 — First pad landed
    if (!bTutorialStep6Shown && Pawn->PadsLanded >= 1)
    {
        bTutorialStep6Shown = true;
        ShowTutorialStep(
            TEXT("FIRST PAD COMPLETE — 1 OF 3"),
            TEXT("Great landing!\n\n"
                "There are 3 landing pads on this level. You need to land on all 3 "
                "to complete the level.\n\n"
                "After each landing your ship will relaunch automatically. "
                "Use the minimap to find the next pad.\n\n"
                "Good luck.")
        );
        return;
    }
}