#include "CockpitWidget.h"
#include "Blueprint/UserWidget.h"

void UCockpitWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCockpitWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Slowly drain fuel over time for testing
    FuelLevel = FMath::Clamp(FuelLevel - (InDeltaTime * 0.01f), 0.0f, 1.0f);
}

// --- Helper functions (following professor's pattern) ---

void UCockpitWidget::DrawFuelGauge()
{
    // Fuel gauge drawing code will go here
    // FuelLevel drives this (0.0 to 1.0)
}

void UCockpitWidget::DrawThrottleGauge()
{
    // Throttle gauge drawing code will go here
    // ThrottleLevel drives this (0.0 to 1.0)
}

void UCockpitWidget::DrawMathDisplay()
{
    // Math problem display drawing code will go here
    // CurrentProblem string displays here
}

void UCockpitWidget::DrawKeypad()
{
    // Keypad button drawing code will go here
}

void UCockpitWidget::DrawRadar()
{
    // Radar/map drawing code will go here
}

void UCockpitWidget::DrawAltimeter()
{
    // Altitude meter drawing code will go here
    // Altitude variable (0-100) drives this
}