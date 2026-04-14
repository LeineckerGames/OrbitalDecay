#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CockpitWidget.generated.h"

UCLASS()
class ORBITALDECAY_API UCockpitWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Called every frame to draw the HUD
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Fuel level 0.0 = empty, 1.0 = full
    void SetFuelLevel(float NewFuelLevel) { FuelLevel = NewFuelLevel; }

    // Throttle level 0.0 = none, 1.0 = max
    void SetThrottleLevel(float NewThrottleLevel) { ThrottleLevel = NewThrottleLevel; }

    // Set the math problem text
    void SetMathProblem(const FString& Problem) { CurrentProblem = Problem; }

protected:
    // Called when widget is created
    virtual void NativeConstruct() override;

private:
    // --- Draw helper functions (per your professor's instructions) ---
    void DrawFuelGauge();
    void DrawThrottleGauge();
    void DrawMathDisplay();
    void DrawKeypad();
    void DrawRadar();
    void DrawAltimeter();

    // --- Game state variables ---
    float FuelLevel = 1.0f;       // Starts full
    float ThrottleLevel = 0.5f;   // Starts at half
    float Altitude = 50.0f;       // 0-100
    FString CurrentProblem = TEXT("5 + 7 = ?");
};