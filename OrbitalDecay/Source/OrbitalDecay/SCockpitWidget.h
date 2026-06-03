#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/Input/SEditableText.h"

class AMyHUD;

// ═══════════════════════════════════════════════════════════════════
// SMinimapWidget
// Ship-relative radar: ship arrow fixed at center pointing up.
// World (landing pads) rotates and translates around it.
// ═══════════════════════════════════════════════════════════════════
class SMinimapWidget : public SLeafWidget
{
public:
    SLATE_BEGIN_ARGS(SMinimapWidget) : _OwnerHUD() {}
        SLATE_ARGUMENT(TWeakObjectPtr<AMyHUD>, OwnerHUD)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    virtual int32 OnPaint(
        const FPaintArgs& Args,
        const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements,
        int32                    LayerId,
        const FWidgetStyle& InWidgetStyle,
        bool                     bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override
    {
        return FVector2D(140.f, 140.f);
    }

private:
    TWeakObjectPtr<AMyHUD> MyOwnerHUD;
};

// ═══════════════════════════════════════════════════════════════════
// SCockpitWidget
// ═══════════════════════════════════════════════════════════════════
class ORBITALDECAY_API SCockpitWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCockpitWidget) : _OwnerHUD() {}
        SLATE_ARGUMENT(TWeakObjectPtr<AMyHUD>, OwnerHUD)
    SLATE_END_ARGS()

    void SetInputEnabled(bool bEnabled);
    void SetPauseButtonVisible(bool bVisible);
    void Construct(const FArguments& InArgs);
    virtual bool SupportsKeyboardFocus() const override { return true; }
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    TWeakObjectPtr<AMyHUD> MyOwnerHUD;
    TSharedPtr<SEditableText> AnswerInputBox;
    TSharedPtr<SButton> PauseButton;
    FText       ResultText;
    FSlateColor ResultColor;
    bool        bInputEnabled = true;

    FString FullMissionText;
    FString DisplayedMissionText;
    float TypewriterAccumulator = 0.0f;
    float CharsPerSecond = 20.0f;
    bool bTypewriterActive = false;
    int32 TypewriterIndex = 0;

    TSharedRef<SWidget> BuildWindowArea();
    TSharedRef<SWidget> BuildBottomPanel();
    TSharedRef<SWidget> BuildLeftPanel();
    TSharedRef<SWidget> BuildCenterPanel();
    TSharedRef<SWidget> BuildRightPanel();
    TSharedRef<SWidget> BuildFuelGauge();
    TSharedRef<SWidget> BuildThrottleIndicator();
    TSharedRef<SWidget> BuildThrustSwitch();
    TSharedRef<SWidget> BuildComputerDisplay();
    TSharedRef<SWidget> BuildKeypad();
    TSharedRef<SWidget> BuildMinimap();
    TSharedRef<SWidget> BuildAltitudePanel();
    TSharedRef<SWidget> BuildMissionPanel();
    TSharedRef<SWidget> BuildKeypadButton(FString Label);

    FReply OnKeypadButtonClicked(FString Label);
    FReply OnAnswerCommitted(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
    void   CheckAnswer();
    void   AppendToInput(FString Character);
};