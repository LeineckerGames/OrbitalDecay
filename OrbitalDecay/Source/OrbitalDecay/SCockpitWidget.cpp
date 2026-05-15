#include "SCockpitWidget.h"
#include "MyHUD.h"
#include "ALanderPawn.h"
#include "OrbitalDecayGameMode.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SImage.h"
#include "Framework/Application/SlateApplication.h"
#include "Containers/Ticker.h"

// ─── Color palette ───────────────────────────────────────────────
static const FLinearColor C_PanelBg = FLinearColor(0.05f, 0.06f, 0.08f, 1.f);   // dark navy
static const FLinearColor C_PanelBorder = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);   // steel blue
static const FLinearColor C_WindowBg = FLinearColor(0.02f, 0.04f, 0.06f, 0.85f); // near black
static const FLinearColor C_FuelFull = FLinearColor(0.10f, 0.90f, 0.20f, 1.f);   // green
static const FLinearColor C_FuelLow = FLinearColor(0.95f, 0.20f, 0.10f, 1.f);   // red
static const FLinearColor C_DisplayBg = FLinearColor(0.02f, 0.08f, 0.04f, 1.f);   // dark green screen
static const FLinearColor C_DisplayText = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);   // phosphor green
static const FLinearColor C_ButtonBg = FLinearColor(0.10f, 0.12f, 0.15f, 1.f);   // dark button
static const FLinearColor C_ButtonText = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);   // light blue-white
static const FLinearColor C_MinimapBg = FLinearColor(0.03f, 0.06f, 0.03f, 1.f);   // dark radar green
static const FLinearColor C_MinimapGrid = FLinearColor(0.10f, 0.35f, 0.10f, 1.f);   // radar grid
static const FLinearColor C_AccentGreen = FLinearColor(0.10f, 0.90f, 0.20f, 1.f);   // bright green
static const FLinearColor C_AccentAmber = FLinearColor(1.00f, 0.60f, 0.00f, 1.f);   // amber warning
static const FLinearColor C_White = FLinearColor::White;
static const FLinearColor C_Transparent = FLinearColor(0, 0, 0, 0);

// ─── Construct ───────────────────────────────────────────────────
void SCockpitWidget::Construct(const FArguments& InArgs)
{
    MyOwnerHUD = InArgs._OwnerHUD;
    ResultText = FText::GetEmpty();
    ResultColor = FSlateColor(C_White);

    ChildSlot
        [
            // Root: full screen vertical split
            SNew(SVerticalBox)

                // TOP: Window area (60% of screen height)
                + SVerticalBox::Slot()
                .FillHeight(0.60f)
                [
                    BuildWindowArea()
                ]

                // BOTTOM: Control panels (40% of screen height)
                + SVerticalBox::Slot()
                .FillHeight(0.40f)
                [
                    BuildBottomPanel()
                ]
        ];
}

// ─── Window Area ─────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildWindowArea()
{
    return SNew(SOverlay)

        // Fully transparent window - game world shows through
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SSpacer)
        ]

        // Result feedback text at bottom of window
        + SOverlay::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Bottom)
        .Padding(0, 0, 0, 20)
        [
            SNew(STextBlock)
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 28))
                .ColorAndOpacity_Lambda([this]() { return ResultColor; })
                .Text_Lambda([this]() { return ResultText; })
        ];
}

// ─── Bottom Panel ─────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildBottomPanel()
{
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBg)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(8.f)
        [
            SNew(SHorizontalBox)

                // LEFT panel (25%)
                + SHorizontalBox::Slot()
                .FillWidth(0.25f)
                .Padding(4.f)
                [
                    BuildLeftPanel()
                ]

                // CENTER panel (50%)
                + SHorizontalBox::Slot()
                .FillWidth(0.50f)
                .Padding(4.f)
                [
                    BuildCenterPanel()
                ]

                // RIGHT panel (25%)
                + SHorizontalBox::Slot()
                .FillWidth(0.25f)
                .Padding(4.f)
                [
                    BuildRightPanel()
                ]
        ];
}

// ─── Left Panel ───────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildLeftPanel()
{
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(6.f)
        [
            SNew(SVerticalBox)

                // FUEL label
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 4, 0, 2)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("FUEL")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                        .ColorAndOpacity(C_AccentGreen)
                ]

                // Fuel gauge
                + SVerticalBox::Slot()
                .FillHeight(1.f)
                .Padding(0, 2)
                [
                    BuildFuelGauge()
                ]

                // Divider
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 6)
                [
                    SNew(SBorder)
                        .BorderBackgroundColor(C_PanelBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .Padding(FMargin(0, 1))
                        [
                            SNew(SSpacer)
                        ]
                ]

            // THROTTLE label
            + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 2)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("THROTTLE")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                        .ColorAndOpacity(C_AccentAmber)
                ]

                // Throttle indicator
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 4)
                [
                    BuildThrottleIndicator()
                ]

                // Thrust switch
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 4)
                [
                    BuildThrustSwitch()
                ]
        ];
}

// ─── Fuel Gauge ───────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildFuelGauge()
{
    // Vertical bar gauge - fill from bottom
    // Placeholder: colored SBorder that scales with fuel level
    // Real art: fuel bar frame PNG + fill texture will slot in here
    return SNew(SBox)
        .WidthOverride(40.f)
        [
            SNew(SBorder)
                .BorderBackgroundColor(C_PanelBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .Padding(3.f)
                [
                    SNew(SOverlay)

                        // Background track
                        + SOverlay::Slot()
                        [
                            SNew(SBorder)
                                .BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.f))
                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        ]

                        // Fill bar - driven by fuel level
                        + SOverlay::Slot()
                        .VAlign(VAlign_Bottom)
                        [
                            SNew(SBox)
                                .HeightOverride_Lambda([this]() -> FOptionalSize
                                    {
                                        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                        {
                                            ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                            if (Pawn)
                                            {
                                                float FuelPct = FMath::Clamp(Pawn->Fuel / 1000.f, 0.f, 1.f);
                                                return FOptionalSize(FuelPct * 150.f);
                                            }
                                        }
                                        return FOptionalSize(150.f);
                                    })
                                [
                                    SNew(SBorder)
                                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                        .BorderBackgroundColor_Lambda([this]() -> FLinearColor
                                            {
                                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                                {
                                                    ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                                    if (Pawn)
                                                    {
                                                        return Pawn->Fuel < 100.f ? C_FuelLow : C_FuelFull;
                                                    }
                                                }
                                                return C_FuelFull;
                                            })
                                ]
                        ]
                ]
        ];
}

// ─── Throttle Indicator ───────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildThrottleIndicator()
{
    // Placeholder: colored circle/box showing ON or OFF
    // Real art: thruster ON/OFF icon PNGs will replace this
    return SNew(SBox)
        .WidthOverride(36.f)
        .HeightOverride(36.f)
        [
            SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor_Lambda([this]() -> FLinearColor
                    {
                        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                        {
                            ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                            if (Pawn) return Pawn->Fuel > 0.f ? C_AccentGreen : C_FuelLow;
                        }
                        return C_AccentGreen;
                    })
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("ON")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                        .ColorAndOpacity(C_PanelBg)
                        .Justification(ETextJustify::Center)
                ]
        ];
}

// ─── Thrust Switch ────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildThrustSwitch()
{
    return SNew(SButton)
        .ButtonColorAndOpacity(C_Transparent)
        .OnClicked_Lambda([this]() -> FReply
            {
                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                {
                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                    if (P)
                    {
                        P->ToggleThrustMode();
                        ResultText = FText::FromString(P->bForwardThrustMode ?
                            TEXT("FORWARD THRUST") : TEXT("VERTICAL THRUST"));
                        ResultColor = FSlateColor(C_AccentAmber);
                    }
                }
                return FReply::Handled();
            })
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("VERTICAL")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
                        .ColorAndOpacity_Lambda([this]() -> FSlateColor
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return P->bForwardThrustMode ?
                                        FSlateColor(C_PanelBorder) : FSlateColor(C_AccentGreen);
                                }
                                return FSlateColor(C_AccentGreen);
                            })
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 2)
                [
                    SNew(SBox)
                        .WidthOverride(20.f)
                        .HeightOverride(40.f)
                        [
                            SNew(SOverlay)

                                + SOverlay::Slot()
                                [
                                    SNew(SBorder)
                                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                        .BorderBackgroundColor(C_PanelBorder)
                                ]

                                + SOverlay::Slot()
                                .VAlign(VAlign_Top)
                                [
                                    SNew(SBox)
                                        .HeightOverride(18.f)
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                                .BorderBackgroundColor_Lambda([this]() -> FLinearColor
                                                    {
                                                        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                                        {
                                                            ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                                            if (P) return P->bForwardThrustMode ?
                                                                C_PanelBorder : C_AccentAmber;
                                                        }
                                                        return C_AccentAmber;
                                                    })
                                        ]
                                ]

                            + SOverlay::Slot()
                                .VAlign(VAlign_Bottom)
                                [
                                    SNew(SBox)
                                        .HeightOverride(18.f)
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                                .BorderBackgroundColor_Lambda([this]() -> FLinearColor
                                                    {
                                                        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                                        {
                                                            ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                                            if (P) return P->bForwardThrustMode ?
                                                                C_AccentAmber : C_PanelBorder;
                                                        }
                                                        return C_PanelBorder;
                                                    })
                                        ]
                                ]
                        ]
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("FORWARD")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
                        .ColorAndOpacity_Lambda([this]() -> FSlateColor
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return P->bForwardThrustMode ?
                                        FSlateColor(C_AccentGreen) : FSlateColor(C_PanelBorder);
                                }
                                return FSlateColor(C_PanelBorder);
                            })
                ]
        ];
}

// ─── Center Panel ─────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildCenterPanel()
{
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(6.f)
        [
            SNew(SVerticalBox)

                // Computer display top
                + SVerticalBox::Slot()
                .FillHeight(0.35f)
                .Padding(0, 0, 0, 6)
                [
                    BuildComputerDisplay()
                ]

                // Keypad bottom
                + SVerticalBox::Slot()
                .FillHeight(0.65f)
                [
                    BuildKeypad()
                ]
        ];
}

// ─── Computer Display ─────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildComputerDisplay()
{
    // Placeholder: dark green screen with phosphor text
    // Real art: computer screen background + terminal frame PNG
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(4.f)
        [
            SNew(SBorder)
                .BorderBackgroundColor(C_DisplayBg)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .Padding(12.f)
                [
                    SNew(SVerticalBox)

                        // Math question
                        + SVerticalBox::Slot()
                        .FillHeight(1.f)
                        .VAlign(VAlign_Center)
                        .HAlign(HAlign_Center)
                        [
                            SNew(STextBlock)
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 32))
                                .ColorAndOpacity(C_DisplayText)
                                .Text_Lambda([this]() -> FText
                                    {
                                        if (MyOwnerHUD.IsValid())
                                        {
                                            FString Q = MyOwnerHUD->CurrentQuestionText;
                                            if (Q.IsEmpty()) return FText::FromString(TEXT("PRESS A S M D"));
                                            return FText::FromString(Q);
                                        }
                                        return FText::FromString(TEXT("NO HUD"));
                                    })
                        ]

                    // Answer input
                    + SVerticalBox::Slot()
                        .AutoHeight()
                        .HAlign(HAlign_Center)
                        [
                            SNew(SBox)
                                .WidthOverride(200.f)
                                [
                                    SAssignNew(AnswerInputBox, SEditableText)
                                        .HintText(FText::FromString(TEXT("_ _ _")))
                                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 28))
                                        .ColorAndOpacity(C_DisplayText)
                                        .IsReadOnly_Lambda([this]()
                                            {
                                                return MyOwnerHUD.IsValid() ? !MyOwnerHUD->bQuestionActive : true;
                                            })
                                        .OnKeyDownHandler(this, &SCockpitWidget::OnAnswerCommitted)
                                ]
                        ]
                ]
        ];
}

// ─── Keypad ───────────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildKeypad()
{
    // 4 rows: [7 8 9] [4 5 6] [1 2 3] [0 ENTER]
    // Real art: button texture + pressed texture will replace colored boxes
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(4.f)
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot().FillHeight(1.f).Padding(2.f)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("7")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("8")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("9")]
                ]

                + SVerticalBox::Slot().FillHeight(1.f).Padding(2.f)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("4")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("5")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("6")]
                ]

                + SVerticalBox::Slot().FillHeight(1.f).Padding(2.f)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("1")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("2")]
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("3")]
                ]

                + SVerticalBox::Slot().FillHeight(1.f).Padding(2.f)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot().FillWidth(1.f).Padding(2.f)[BuildKeypadButton("0")]
                        + SHorizontalBox::Slot().FillWidth(2.f).Padding(2.f)[BuildKeypadButton("ENTER")]
                ]
        ];
}

// ─── Single Keypad Button ─────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildKeypadButton(FString Label)
{
    return SNew(SButton)
        .ButtonColorAndOpacity(C_ButtonBg)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .OnClicked_Lambda([this, Label]() -> FReply
            {
                return OnKeypadButtonClicked(Label);
            })
        [
            SNew(STextBlock)
                .Text(FText::FromString(Label))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
                .ColorAndOpacity(C_ButtonText)
                .Justification(ETextJustify::Center)
        ];
}

// ─── Keypad Click Handler ─────────────────────────────────────────
FReply SCockpitWidget::OnKeypadButtonClicked(FString Label)
{
    if (Label == "ENTER")
    {
        CheckAnswer();
        return FReply::Handled();
    }

    // Start a question if none active
    if (!MyOwnerHUD.IsValid()) return FReply::Unhandled();

    if (!MyOwnerHUD->bQuestionActive)
    {
        // Default to addition if no question active
        AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(
            MyOwnerHUD->GetWorld()->GetAuthGameMode());
        int32 Level = GM ? GM->GlobalLevel : 1;
        MyOwnerHUD->GenerateNewQuestion(MyOwnerHUD->QuestionType, Level);
        MyOwnerHUD->bQuestionActive = true;
    }

    AppendToInput(Label);
    return FReply::Handled();
}

// ─── Append To Input ──────────────────────────────────────────────
void SCockpitWidget::AppendToInput(FString Character)
{
    if (!AnswerInputBox.IsValid()) return;
    FString Current = AnswerInputBox->GetText().ToString();
    if (Current.Len() < 6) // max 6 digits
    {
        AnswerInputBox->SetText(FText::FromString(Current + Character));
    }
}

// ─── Right Panel ──────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildRightPanel()
{
    return SNew(SBorder)
        .BorderBackgroundColor(C_PanelBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(6.f)
        [
            SNew(SVerticalBox)

                // Minimap top
                + SVerticalBox::Slot()
                .FillHeight(0.60f)
                .HAlign(HAlign_Center)
                .Padding(0, 4)
                [
                    BuildMinimap()
                ]

                // Altitude + speed bottom
                + SVerticalBox::Slot()
                .FillHeight(0.40f)
                .Padding(0, 4)
                [
                    BuildAltitudePanel()
                ]
        ];
}

// ─── Minimap ──────────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildMinimap()
{
    // Placeholder: dark circle with grid lines effect using nested boxes
    // Real art: circular map frame PNG + player icon PNG
    return SNew(SBox)
        .WidthOverride(140.f)
        .HeightOverride(140.f)
        [
            SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .BorderBackgroundColor(C_PanelBorder)
                .Padding(4.f)
                [
                    SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(C_MinimapBg)
                        .Padding(8.f)
                        [
                            SNew(SOverlay)

                                // Grid cross lines placeholder
                                + SOverlay::Slot()
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Fill)
                                [
                                    SNew(SBox)
                                        .WidthOverride(1.f)
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                                .BorderBackgroundColor(C_MinimapGrid)
                                        ]
                                ]

                            + SOverlay::Slot()
                                .HAlign(HAlign_Fill)
                                .VAlign(VAlign_Center)
                                [
                                    SNew(SBox)
                                        .HeightOverride(1.f)
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                                .BorderBackgroundColor(C_MinimapGrid)
                                        ]
                                ]

                            // Player dot in center
                            + SOverlay::Slot()
                                .HAlign(HAlign_Center)
                                .VAlign(VAlign_Center)
                                [
                                    SNew(SBox)
                                        .WidthOverride(8.f)
                                        .HeightOverride(8.f)
                                        [
                                            SNew(SBorder)
                                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                                .BorderBackgroundColor(C_AccentGreen)
                                        ]
                                ]
                        ]
                ]
        ];
}

// ─── Altitude Panel ───────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildAltitudePanel()
{
    // Placeholder: text readouts for altitude and vertical speed
    // Real art: panel background PNG
    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(C_PanelBg)
        .Padding(8.f)
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
                        .ColorAndOpacity(C_White)
                        .Text_Lambda([this]() -> FText
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return FText::FromString(
                                        FString::Printf(TEXT("ALT: %.0f ft"), P->CurrentAltitude));
                                }
                                return FText::FromString(TEXT("ALT: --"));
                            })
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 4)
                [
                    SNew(STextBlock)
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
                        .ColorAndOpacity_Lambda([this]() -> FSlateColor
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return P->CurrentVelocity.Z < -5.f ?
                                        FSlateColor(C_FuelLow) : FSlateColor(C_AccentGreen);
                                }
                                return FSlateColor(C_White);
                            })
                        .Text_Lambda([this]() -> FText
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return FText::FromString(
                                        FString::Printf(TEXT("VSPD: %.1f"), P->CurrentVelocity.Z));
                                }
                                return FText::FromString(TEXT("VSPD: --"));
                            })
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 4)
                [
                    SNew(STextBlock)
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
                        .ColorAndOpacity(C_White)
                        .Text_Lambda([this]() -> FText
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return FText::FromString(
                                        FString::Printf(TEXT("FUEL: %.0f"), P->Fuel));
                                }
                                return FText::FromString(TEXT("FUEL: --"));
                            })
                ]
        ];
}

// ─── Input Enable/Disable ─────────────────────────────────────────
void SCockpitWidget::SetInputEnabled(bool bEnabled)
{
    // Blocks all player input during replay, success, and failure states
    bInputEnabled = bEnabled;
}

// ─── Key Input (keyboard fallback) ────────────────────────────────
FReply SCockpitWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (!MyOwnerHUD.IsValid()) return FReply::Unhandled();
    if (!bInputEnabled) return FReply::Handled();

    if (InKeyEvent.GetKey() == EKeys::L)
    {
        ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
        if (P)
        {
            P->ToggleThrustMode();
            ResultText = FText::FromString(P->bForwardThrustMode ?
                TEXT("FORWARD THRUST") : TEXT("VERTICAL THRUST"));
            ResultColor = FSlateColor(C_AccentAmber);
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
            AOrbitalDecayGameMode* GM = Cast<AOrbitalDecayGameMode>(
                MyOwnerHUD->GetWorld()->GetAuthGameMode());
            int32 Level = GM ? GM->GlobalLevel : 1;
            MyOwnerHUD->GenerateNewQuestion(Type, Level);
            MyOwnerHUD->bQuestionActive = true;
            ResultText = FText::GetEmpty();

            if (AnswerInputBox.IsValid())
            {
                FSlateApplication::Get().SetKeyboardFocus(AnswerInputBox);
                FTSTicker::GetCoreTicker().AddTicker(
                    FTickerDelegate::CreateLambda([this](float) -> bool
                        {
                            if (AnswerInputBox.IsValid())
                                AnswerInputBox->SetText(FText::GetEmpty());
                            return false;
                        }));
            }
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
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

// ─── Check Answer ─────────────────────────────────────────────────
void SCockpitWidget::CheckAnswer()
{
    if (!MyOwnerHUD.IsValid() || !AnswerInputBox.IsValid()) return;
    if (!bInputEnabled) return;

    FString InputStr = AnswerInputBox->GetText().ToString();
    int32 PlayerAnswer = FCString::Atoi(*InputStr);

    if (PlayerAnswer == MyOwnerHUD->CurrentCorrectAnswer)
    {
        ResultText = FText::FromString(TEXT("CORRECT!"));
        ResultColor = FSlateColor(C_AccentGreen);
        MyOwnerHUD->bQuestionActive = false;
        MyOwnerHUD->CurrentQuestionText = TEXT("");

        ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
        if (P)
        {
            if (MyOwnerHUD->QuestionType == "d")
            {
                P->AddFuel(MyOwnerHUD->FuelRewardAmount);
                ResultText = FText::FromString(
                    FString::Printf(TEXT("CORRECT! +%.0f FUEL"), MyOwnerHUD->FuelRewardAmount));
            }
            else if (MyOwnerHUD->QuestionType == "m")
            {
                P->ActivateBoost();
                ResultText = FText::FromString(P->bForwardThrustMode ?
                    TEXT("CORRECT! FORWARD BOOST!") : TEXT("CORRECT! BOOST!"));
            }
            else if (MyOwnerHUD->QuestionType == "a")
            {
                P->RotateLeft();
                ResultText = FText::FromString(TEXT("CORRECT! ROTATING LEFT"));
                ResultColor = FSlateColor(C_AccentAmber);
            }
            else if (MyOwnerHUD->QuestionType == "s")
            {
                P->RotateRight();
                ResultText = FText::FromString(TEXT("CORRECT! ROTATING RIGHT"));
                ResultColor = FSlateColor(C_AccentAmber);
            }
        }
    }
    else
    {
        ResultText = FText::FromString(
            FString::Printf(TEXT("WRONG! GOT %d"), PlayerAnswer));
        ResultColor = FSlateColor(C_FuelLow);
    }

    AnswerInputBox->SetText(FText::GetEmpty());
    FSlateApplication::Get().SetKeyboardFocus(AsShared());
}