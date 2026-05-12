#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableText.h"

class AMyHUD;

class ORBITALDECAY_API SCockpitWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCockpitWidget) : _OwnerHUD() {}
        SLATE_ARGUMENT(TWeakObjectPtr<AMyHUD>, OwnerHUD)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual bool SupportsKeyboardFocus() const override { return true; }
    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

private:
    TWeakObjectPtr<AMyHUD> MyOwnerHUD;
    TSharedPtr<SEditableText> AnswerInputBox;
    FText ResultText;
    FSlateColor ResultColor;

    // Helper functions - one per section (professor's pattern)
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
    void CheckAnswer();
    void AppendToInput(FString Character);
};