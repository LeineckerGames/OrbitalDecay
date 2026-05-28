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

private:
    TWeakObjectPtr<AMyHUD> MyOwnerHUD;
    TSharedPtr<SEditableText> AnswerInputBox;
    TSharedPtr<SButton> PauseButton;
    FText       ResultText;
    FSlateColor ResultColor;
    bool        bInputEnabled = true;

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
    TSharedRef<SWidget> BuildKeypadButton(FString Label);

    FReply OnKeypadButtonClicked(FString Label);
    FReply OnAnswerCommitted(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
    void   CheckAnswer();
    void   AppendToInput(FString Character);
};