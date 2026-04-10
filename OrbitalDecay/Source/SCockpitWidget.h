#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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

    FReply OnAnswerCommitted(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
    void CheckAnswer();
};