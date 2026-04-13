#include "SCockpitWidget.h"
#include "MyHUD.h"
#include "ALanderPawn.h"
#include "OrbitalDecayGameMode.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "Containers/Ticker.h"

void SCockpitWidget::Construct(const FArguments& InArgs)
{
    MyOwnerHUD = InArgs._OwnerHUD;
    ResultText = FText::GetEmpty();
    ResultColor = FSlateColor(FLinearColor::White);

    ChildSlot
        [
            SNew(SOverlay)
                + SOverlay::Slot() // Cockpit image layer
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                [
                    SNew(SImage)
                        .Image(MyOwnerHUD.IsValid() ? &MyOwnerHUD->CockpitBrush : nullptr)
                ]
                + SOverlay::Slot() // Math question + input layer
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(0, 90, 0, 0)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity(FLinearColor::Green)
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() {
                                return FText::FromString(MyOwnerHUD.IsValid() ? MyOwnerHUD->CurrentQuestionText : "No HUD");
                                    })
                        ]
                    + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(10, 0, 0, 0)
                        [
                            SNew(SBox)
                                .WidthOverride(200.f)
                                [
                                    SAssignNew(AnswerInputBox, SEditableText)
                                        .HintText(FText::FromString(TEXT("...")))
                                        .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                        .ColorAndOpacity(FLinearColor::Green)
                                        .IsReadOnly_Lambda([this]() {
                                        return MyOwnerHUD.IsValid() ? !MyOwnerHUD->bQuestionActive : true;
                                            })
                                        .OnKeyDownHandler(this, &SCockpitWidget::OnAnswerCommitted)
                                ]
                        ]
                ]
            + SOverlay::Slot() // Result text layer
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(0, 200, 0, 0)
                [
                    SNew(STextBlock)
                        .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                        .ColorAndOpacity_Lambda([this]() { return ResultColor; })
                        .Text_Lambda([this]() { return ResultText; })
                ]
                + SOverlay::Slot() // Velocity + Altitude layer
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Top)
                .Padding(20, 20, 0, 0)
                [
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity(FLinearColor::White)
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() -> FText {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        FVector Vel = MyPawn->CurrentVelocity;
                                        return FText::FromString(FString::Printf(TEXT("Velocity: Z=%.1f"), Vel.Z));
                                    }
                                }
                                return FText::FromString(TEXT("Velocity: --"));
                                    })
                        ]
                    + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity(FLinearColor::White)
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() -> FText {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        return FText::FromString(FString::Printf(TEXT("Altitude: %.0f ft"), MyPawn->CurrentAltitude));
                                    }
                                }
                                return FText::FromString(TEXT("Altitude: --"));
                                    })
                        ]
                    + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity_Lambda([this]() -> FSlateColor {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        // Turn red when fuel is low
                                        return MyPawn->Fuel < 100.0f ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White);
                                    }
                                }
                                return FSlateColor(FLinearColor::White);
                                    })
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() -> FText {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        return FText::FromString(FString::Printf(TEXT("Fuel: %.0f"), MyPawn->Fuel));
                                    }
                                }
                                return FText::FromString(TEXT("Fuel: --"));
                                    })
                        ]
                    + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity_Lambda([this]() -> FSlateColor {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        return MyPawn->bForwardThrustMode ?
                                            FSlateColor(FLinearColor::Yellow) :
                                            FSlateColor(FLinearColor::Green);
                                    }
                                }
                                return FSlateColor(FLinearColor::White);
                                    })
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() -> FText {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        return FText::FromString(
                                            MyPawn->bForwardThrustMode ?
                                            TEXT("Mode: FORWARD") :
                                            TEXT("Mode: VERTICAL")
                                        );
                                    }
                                }
                                return FText::FromString(TEXT("Mode: --"));
                                    })
                        ]
                    + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(STextBlock)
                                .ShadowColorAndOpacity(FLinearColor::Black)
                                .ShadowOffset(FVector2D(2, 2))
                                .ColorAndOpacity(FLinearColor::White)
                                .Font(MyOwnerHUD.IsValid() ? MyOwnerHUD->QuestionFont : FCoreStyle::Get().GetFontStyle("EmbossedText"))
                                .Text_Lambda([this]() -> FText {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (MyPawn)
                                    {
                                        float Heading = MyPawn->GetActorRotation().Yaw;
                                        return FText::FromString(FString::Printf(TEXT("Heading: %.0f�"), Heading));
                                    }
                                }
                                return FText::FromString(TEXT("Heading: --"));
                                    })
                        ]
                ]
                    
        ];
}

FReply SCockpitWidget::OnAnswerCommitted(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        CheckAnswer();
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply SCockpitWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (!MyOwnerHUD.IsValid()) return FReply::Unhandled();

    // L key toggles thrust mode regardless of question state
    if (InKeyEvent.GetKey() == EKeys::L)
    {
        ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
        if (MyPawn)
        {
            MyPawn->ToggleThrustMode();
            ResultText = FText::FromString(
                MyPawn->bForwardThrustMode ?
                TEXT("Forward Thrust Mode") :
                TEXT("Vertical Thrust Mode")
            );
            ResultColor = FSlateColor(FLinearColor::Yellow);
        }
        return FReply::Handled();
    }


    if (!MyOwnerHUD->bQuestionActive)
    {
        FString Type = TEXT("");

        if (InKeyEvent.GetKey() == EKeys::A) Type = "a";
        else if (InKeyEvent.GetKey() == EKeys::S) Type = "s";
        else if (InKeyEvent.GetKey() == EKeys::M) Type = "m";
        else if (InKeyEvent.GetKey() == EKeys::D) Type = "d";

        if (!Type.IsEmpty())
        {
            AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(MyOwnerHUD->GetWorld()->GetAuthGameMode());
            int32 Level = GM ? GM->GlobalLevel : 1;
            MyOwnerHUD->GenerateNewQuestion(Type, Level);
            MyOwnerHUD->bQuestionActive = true;
            ResultText = FText::GetEmpty();

            if (AnswerInputBox.IsValid())
            {
                FSlateApplication::Get().SetKeyboardFocus(AnswerInputBox);

                // Defer the clear to next frame so it happens after the key event is processed
                FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime) -> bool
                    {
                        if (AnswerInputBox.IsValid())
                        {
                            AnswerInputBox->SetText(FText::GetEmpty());
                        }
                        return false; // Return false so it only fires once
                    }));
            }
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
}

void SCockpitWidget::CheckAnswer()
{
    if (!MyOwnerHUD.IsValid() || !AnswerInputBox.IsValid()) return;

    FString InputStr = AnswerInputBox->GetText().ToString();
    int32 PlayerAnswer = FCString::Atoi(*InputStr);

    if (PlayerAnswer == MyOwnerHUD->CurrentCorrectAnswer)
    {
        ResultText = FText::FromString(TEXT("Correct!"));
        ResultColor = FSlateColor(FLinearColor::Green);
        MyOwnerHUD->bQuestionActive = false;
        MyOwnerHUD->CurrentQuestionText = TEXT("Press A, S, M or D for a question");

        ALanderPawn* MyPawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
        if (MyPawn)
        {
            if (MyOwnerHUD->QuestionType == "d")
            {
                MyPawn->AddFuel(MyOwnerHUD->FuelRewardAmount);
                ResultText = FText::FromString(FString::Printf(TEXT("Correct! +%.0f Fuel"), MyOwnerHUD->FuelRewardAmount));
            }
            else if (MyOwnerHUD->QuestionType == "m")
            {
                MyPawn->ActivateBoost();
                FString BoostMsg = MyPawn->bForwardThrustMode ?
                    TEXT("Correct! Forward Boost!") :
                    TEXT("Correct! Boost activated!");
                ResultText = FText::FromString(BoostMsg);
            }
            else if (MyOwnerHUD->QuestionType == "a")
            {
                MyPawn->RotateLeft();
                ResultText = FText::FromString(TEXT("Correct! Rotating Left"));
                ResultColor = FSlateColor(FLinearColor::Yellow);
            }
            else if (MyOwnerHUD->QuestionType == "s")
            {
                MyPawn->RotateRight();
                ResultText = FText::FromString(TEXT("Correct! Rotating Right"));
                ResultColor = FSlateColor(FLinearColor::Yellow);
            }
        }
    }
    else
    {
        ResultText = FText::FromString(FString::Printf(TEXT("Wrong! You entered %d"), PlayerAnswer));
        ResultColor = FSlateColor(FLinearColor::Red);
    }

    AnswerInputBox->SetText(FText::GetEmpty());
    FSlateApplication::Get().SetKeyboardFocus(AsShared());
}