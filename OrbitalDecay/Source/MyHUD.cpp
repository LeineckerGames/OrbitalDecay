#include "MyHUD.h"
#include "SCockpitWidget.h"
#include "Widgets/SWeakWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "OrbitalDecayGameMode.h"
#include "ALanderPawn.h"
#include "Engine/Canvas.h"
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

    // Draw velocity in the top-left corner
    ALanderPawn* MyPawn = Cast<ALanderPawn>(GetOwningPawn());
    if (MyPawn)
    {
        FVector Vel = MyPawn->CurrentVelocity;
        FString VelText = FString::Printf(TEXT("Velocity: X=%.1f Y=%.1f Z=%.1f"), Vel.X, Vel.Y, Vel.Z);
        DrawText(VelText, FColor::White, 20.f, 20.f, nullptr, 1.5f);
    }
}

void AMyHUD::SetQuestion(FString Type, int32 Level)
{
    AHUDSetup2GameMode* GM = Cast<AHUDSetup2GameMode>(GetWorld()->GetAuthGameMode());
    if (GM) GM->GlobalLevel = Level; 

    QuestionType = Type;
    CurrentLevel = Level;
    GenerateNewQuestion(QuestionType, CurrentLevel);
}