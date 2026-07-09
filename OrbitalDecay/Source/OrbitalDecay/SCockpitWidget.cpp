#include "SCockpitWidget.h"
#include "MyHUD.h"
#include "ALanderPawn.h"
#include "OrbitalDecayGameMode.h"
#include "EngineUtils.h"
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
#include "Rendering/DrawElements.h"
#include "Engine/TextureRenderTarget2D.h"

// ─── Color palette ────────────────────────────────────────────────
static const FLinearColor C_PanelBg = FLinearColor(0.05f, 0.06f, 0.08f, 1.f);
static const FLinearColor C_PanelBorder = FLinearColor(0.15f, 0.20f, 0.25f, 1.f);
static const FLinearColor C_FuelFull = FLinearColor(0.10f, 0.90f, 0.20f, 1.f);
static const FLinearColor C_FuelLow = FLinearColor(0.95f, 0.20f, 0.10f, 1.f);
static const FLinearColor C_DisplayBg = FLinearColor(0.02f, 0.08f, 0.04f, 1.f);
static const FLinearColor C_DisplayText = FLinearColor(0.20f, 1.00f, 0.30f, 1.f);
static const FLinearColor C_ButtonBg = FLinearColor(0.10f, 0.12f, 0.15f, 1.f);
static const FLinearColor C_ButtonText = FLinearColor(0.85f, 0.90f, 1.00f, 1.f);
static const FLinearColor C_MinimapBg = FLinearColor(0.06f, 0.07f, 0.06f, 1.f);
static const FLinearColor C_AccentGreen = FLinearColor(0.10f, 0.90f, 0.20f, 1.f);
static const FLinearColor C_AccentAmber = FLinearColor(1.00f, 0.60f, 0.00f, 1.f);
static const FLinearColor C_White = FLinearColor::White;
static const FLinearColor C_Transparent = FLinearColor(0, 0, 0, 0);

// Minimap colors
static const FLinearColor C_MapAxis = FLinearColor(0.10f, 0.90f, 0.20f, 0.60f); // green axes
static const FLinearColor C_ShipGreen = FLinearColor(0.10f, 0.90f, 0.20f, 1.f);   // ship arrow
static const FLinearColor C_PadVisited = FLinearColor(0.10f, 0.85f, 0.20f, 1.f); // green
static const FLinearColor C_PadNearby  = FLinearColor(1.00f, 0.85f, 0.00f, 1.f); // yellow
static const FLinearColor C_PadNormal  = FLinearColor(0.55f, 0.05f, 0.05f, 1.f); // dark red
static const FLinearColor C_PadAbove = FLinearColor(1.00f, 0.85f, 0.00f, 1.f);   // yellow when above
static const FLinearColor C_RingColor = FLinearColor(0.10f, 0.35f, 0.10f, 0.50f); // dim ring

// ═══════════════════════════════════════════════════════════════════
// SMinimapWidget — Construct
// ═══════════════════════════════════════════════════════════════════
void SMinimapWidget::Construct(const FArguments& InArgs)
{
    MyOwnerHUD = InArgs._OwnerHUD;
}

// ═══════════════════════════════════════════════════════════════════
// SMinimapWidget — OnPaint
//
// Coordinate system
// ─────────────────
// UE world:    X=forward, Y=right, Z=up
// Ship-local:  rotate world offsets by -ShipYaw so ship-forward
//              always maps to screen-up (positive Y axis on map).
// Screen:      +X=right, +Y=down
//
// Ship-local → screen:
//   screenX = MapCenter.X + ( LocalY / WorldRange) * MapRadius
//   screenY = MapCenter.Y + (-LocalX / WorldRange) * MapRadius
//   (forward=LocalX+ → screen up, right=LocalY+ → screen right)
// ═══════════════════════════════════════════════════════════════════
int32 SMinimapWidget::OnPaint(
    const FPaintArgs&        Args,
    const FGeometry&         AllottedGeometry,
    const FSlateRect&        MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32                    LayerId,
    const FWidgetStyle&      InWidgetStyle,
    bool                     bParentEnabled) const
{
    const FSlateBrush* White = FCoreStyle::Get().GetBrush("WhiteBrush");
    const FVector2D    Size  = AllottedGeometry.GetLocalSize();
    const FVector2D    Ctr   = Size * 0.5f;
    const float        R     = Size.X * 0.5f - 2.f;

    // World units visible to the edge of the radar
    const float WorldRange  = 3000.f;
    // XY distance within which pad turns yellow (approaching)
    const float AboveThresh = 400.f;
    // Pad dot radius in screen pixels
    const float PadRadius   = 9.f;

    // ── 0. Dark background ────────────────────────────────────────
    FSlateDrawElement::MakeBox(
        OutDrawElements, LayerId,
        AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform()),
        White, ESlateDrawEffect::None, C_MinimapBg);
    ++LayerId;

    // ── 1. Green axes ─────────────────────────────────────────────
    FSlateDrawElement::MakeBox(
        OutDrawElements, LayerId,
        AllottedGeometry.ToPaintGeometry(
            FVector2D(1.f, Size.Y),
            FSlateLayoutTransform(FVector2D(Ctr.X, 0.f))),
        White, ESlateDrawEffect::None, C_MapAxis);

    FSlateDrawElement::MakeBox(
        OutDrawElements, LayerId,
        AllottedGeometry.ToPaintGeometry(
            FVector2D(Size.X, 1.f),
            FSlateLayoutTransform(FVector2D(0.f, Ctr.Y))),
        White, ESlateDrawEffect::None, C_MapAxis);
    ++LayerId;

    // ── 2. Bail early if world not ready ─────────────────────────
    if (!MyOwnerHUD.IsValid()) return LayerId;
    ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
    if (!Pawn) return LayerId;
    UWorld* World = Pawn->GetWorld();
    if (!World) return LayerId;

    // ── KEY FIX: Use camera world position as the reference point ─
    // The camera is at -500 X relative to pawn. Using camera position
    // means the minimap center matches exactly what the player sees.
    FVector CamPos = Pawn->Camera
        ? Pawn->Camera->GetComponentLocation()
        : Pawn->GetActorLocation();

    // Use pawn rotation for map orientation (camera inherits this)
    const float YawDeg = Pawn->GetActorRotation().Yaw;
    const float YawRad = FMath::DegreesToRadians(YawDeg);
    const float CosYaw = FMath::Cos(YawRad);
    const float SinYaw = FMath::Sin(YawRad);

    // World position → minimap screen position
    // Reference point is CamPos so the center of the map
    // matches exactly what the camera is looking at
    auto WorldToMap = [&](const FVector& WPos) -> FVector2D
    {
        const float DX = WPos.X - CamPos.X;
        const float DY = WPos.Y - CamPos.Y;

        // Rotate into camera-local space
        const float LX =  CosYaw * DX + SinYaw * DY;
        const float LY = -SinYaw * DX + CosYaw * DY;

        return FVector2D(
            Ctr.X + ( LY / WorldRange) * R,
            Ctr.Y + (-LX / WorldRange) * R);
    };

    // ── 3. Landing pads ───────────────────────────────────────────
    

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor) continue;

        const FString ClassName = Actor->GetClass()->GetName();
        if (!ClassName.Contains(TEXT("LandingPad"))) continue;

        const FVector   PadWorld  = Actor->GetActorLocation();
        const FVector2D PadScreen = WorldToMap(PadWorld);

        // Check if outside visible area
        const float DistFromCtr = FVector2D::Distance(PadScreen, Ctr);
        if (DistFromCtr > R - PadRadius)
        {
            // Edge indicator dot — color must reflect visited state too,
            // otherwise a green (visited) pad turns red once it scrolls
            // off the edge of the radar
            const bool bVisitedEdge = Pawn->VisitedPads.Contains(Actor);
            const FLinearColor EdgeColor = bVisitedEdge ? C_PadVisited : C_PadNormal;

            FVector2D Dir     = (PadScreen - Ctr).GetSafeNormal();
            FVector2D EdgePos = Ctr + Dir * (R - 5.f);
            FSlateDrawElement::MakeBox(
                OutDrawElements, LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(5.f, 5.f),
                    FSlateLayoutTransform(EdgePos - FVector2D(2.5f, 2.5f))),
                White, ESlateDrawEffect::None, EdgeColor);
            continue;
        }

        // Determine color based on visited state and proximity
        // Check visited first — green overrides everything
        const bool bVisited = Pawn->VisitedPads.Contains(Actor);

        // XY distance using camera position for consistency
        const float XYDist = FVector2D::Distance(
            FVector2D(CamPos.X, CamPos.Y),
            FVector2D(PadWorld.X, PadWorld.Y));
        const bool bNearby = !bVisited && XYDist < AboveThresh;

        FLinearColor PadColor;
        if      (bVisited) PadColor = C_PadVisited;
        else if (bNearby)  PadColor = C_PadNearby;
        else               PadColor = C_PadNormal;

        // Draw filled circle using polar coordinate fill
        const int32 CircleSegs = 32;
        for (int32 s = 0; s < CircleSegs; ++s)
        {
            const float A = (float(s) / CircleSegs) * 2.f * PI;
            for (float r = 0.f; r < PadRadius; r += 1.5f)
            {
                const FVector2D Pt(
                    PadScreen.X + r * FMath::Cos(A),
                    PadScreen.Y + r * FMath::Sin(A));
                FSlateDrawElement::MakeBox(
                    OutDrawElements, LayerId,
                    AllottedGeometry.ToPaintGeometry(
                        FVector2D(2.f, 2.f),
                        FSlateLayoutTransform(Pt - FVector2D(1.f))),
                    White, ESlateDrawEffect::None, PadColor);
            }
        }

        // Solid center fill
        FSlateDrawElement::MakeBox(
            OutDrawElements, LayerId,
            AllottedGeometry.ToPaintGeometry(
                FVector2D(PadRadius * 1.4f, PadRadius * 1.4f),
                FSlateLayoutTransform(PadScreen - FVector2D(PadRadius * 0.7f))),
            White, ESlateDrawEffect::None, PadColor);
    }
    ++LayerId;

    // ── 4. Ship dot — fixed at center ────────────────────────────
    {
        const float DotR = 4.f;
        FSlateDrawElement::MakeBox(
            OutDrawElements, LayerId,
            AllottedGeometry.ToPaintGeometry(
                FVector2D(DotR * 2.f, DotR * 2.f),
                FSlateLayoutTransform(Ctr - FVector2D(DotR))),
            White, ESlateDrawEffect::None,
            FLinearColor(0.f, 0.f, 0.f, 1.f));

        // Green ring around dot
        const int32 RingSegs = 16;
        for (int32 i = 0; i < RingSegs; ++i)
        {
            const float A = (float(i) / RingSegs) * 2.f * PI;
            FSlateDrawElement::MakeBox(
                OutDrawElements, LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(2.f, 2.f),
                    FSlateLayoutTransform(FVector2D(
                        Ctr.X + (DotR + 2.f) * FMath::Cos(A) - 1.f,
                        Ctr.Y + (DotR + 2.f) * FMath::Sin(A) - 1.f))),
                White, ESlateDrawEffect::None, C_ShipGreen);
        }
    }
    ++LayerId;

    return LayerId;
}

// ═══════════════════════════════════════════════════════════════════
// SCockpitWidget — Construct
// ═══════════════════════════════════════════════════════════════════
void SCockpitWidget::Construct(const FArguments& InArgs)
{
    MyOwnerHUD = InArgs._OwnerHUD;
    ResultText = FText::GetEmpty();
    ResultColor = FSlateColor(C_White);

    ChildSlot
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .FillHeight(0.60f)
                [BuildWindowArea()]

                + SVerticalBox::Slot()
                .FillHeight(0.40f)
                [BuildBottomPanel()]
        ];
}

// ─── Window Area ──────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildWindowArea()
{
    return SNew(SOverlay)

        // Transparent window — game world shows through
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [ SNew(SSpacer) ]

        // Bottom camera feed — bottom-left of the window, a square
        // "screen" whose bottom edge sits flush against the top of
        // the control panel below, like a physically mounted monitor.
        + SOverlay::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Bottom)
        .Padding(12.f, 0.f, 0.f, 0.f)
        [
            BuildBottomCameraFeed()
        ]

        // Pause button — top right corner
        + SOverlay::Slot()
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Top)
        .Padding(0.f, 8.f, 8.f, 0.f)
        [
            SNew(SBox)
            .WidthOverride(48.f)
            .HeightOverride(48.f)
            [
                SNew(SButton)
                .ButtonColorAndOpacity(FLinearColor(0.08f, 0.10f, 0.14f, 0.85f))
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked_Lambda([this]() -> FReply
                {
                    if (MyOwnerHUD.IsValid())
                    {
                        AMyHUD* HUD = MyOwnerHUD.Get();
                        if (HUD->bIsPaused)
                            HUD->HidePauseScreen();
                        else
                            HUD->ShowPauseScreen();
                    }
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("II")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
                    .ColorAndOpacity(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                    .Justification(ETextJustify::Center)
                ]
            ]
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

// ─── Bottom Camera Feed ─────────────────────────────────────────────
// Square live-render display anchored at the bottom-left of the
// transparent window, sized to ~1/4 of the window's typical width.
// Only visible while the player has the camera toggled on.
TSharedRef<SWidget> SCockpitWidget::BuildBottomCameraFeed()
{
    return SNew(SBox)
        .WidthOverride(220.f)
        .HeightOverride(220.f)
        .Visibility_Lambda([this]() -> EVisibility
        {
            if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
            {
                ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                if (Pawn && Pawn->bShowBottomCamera)
                    return EVisibility::Visible;
            }
            return EVisibility::Hidden;
        })
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
            .BorderBackgroundColor(C_PanelBorder)
            .Padding(4.f)
            [
                SAssignNew(BottomCameraImage, SImage)
                .Image_Lambda([this]() -> const FSlateBrush*
                {
                    static FSlateBrush DynamicBrush;
                    if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                    {
                        ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                        if (Pawn && Pawn->BottomCameraRenderTarget)
                        {
                            DynamicBrush.SetResourceObject(Pawn->BottomCameraRenderTarget);
                            DynamicBrush.ImageSize = FVector2D(220.f, 220.f);
                            return &DynamicBrush;
                        }
                    }
                    return FCoreStyle::Get().GetBrush("WhiteBrush");
                })
            ]
        ];
}

// ─── Bottom Camera Toggle Button ────────────────────────────────────
// Small 48x48 button, same size as the pause button, drawn with a
// simple hand-built camera icon (body + lens) rather than the live
// feed itself — lives in the left control panel next to Throttle.
TSharedRef<SWidget> SCockpitWidget::BuildBottomCameraToggle()
{
    return SAssignNew(BottomCameraToggleButton, SButton)
        .ButtonColorAndOpacity_Lambda([this]() -> FSlateColor
        {
            if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
            {
                ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                if (Pawn && Pawn->bShowBottomCamera)
                    return FSlateColor(FLinearColor(0.10f, 0.30f, 0.16f, 1.f)); // lit green-ish when active
            }
            return FSlateColor(FLinearColor(0.08f, 0.10f, 0.14f, 0.85f)); // default dark, matches pause button
        })
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .OnClicked_Lambda([this]() -> FReply
        {
            if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
            {
                ALanderPawn* Pawn = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                if (Pawn) Pawn->ToggleBottomCamera();
            }
            return FReply::Handled();
        })
        [
            SNew(SBox)
            .WidthOverride(28.f)
            .HeightOverride(22.f)
            [
                SNew(SOverlay)

                // Camera body — rounded-feeling rectangle
                + SOverlay::Slot()
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                [
                    SNew(SBorder)
                    .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                    .BorderBackgroundColor(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                ]

                // Small "viewfinder bump" on top
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Top)
                .Padding(0.f, -5.f, 0.f, 0.f)
                [
                    SNew(SBox)
                    .WidthOverride(10.f)
                    .HeightOverride(5.f)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor(0.20f, 1.00f, 0.30f, 1.f))
                    ]
                ]

                // Lens — dark circle approximation in the center
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SBox)
                    .WidthOverride(12.f)
                    .HeightOverride(12.f)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor(0.05f, 0.06f, 0.08f, 1.f))
                    ]
                ]

                // Inner lens glint
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(0.f, -2.f, 2.f, 0.f)
                [
                    SNew(SBox)
                    .WidthOverride(4.f)
                    .HeightOverride(4.f)
                    [
                        SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor(0.60f, 0.95f, 0.70f, 0.8f))
                    ]
                ]
            ]
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

                + SHorizontalBox::Slot().FillWidth(0.25f).Padding(4.f)
                [BuildLeftPanel()]

                + SHorizontalBox::Slot().FillWidth(0.50f).Padding(4.f)
                [BuildCenterPanel()]

                + SHorizontalBox::Slot().FillWidth(0.25f).Padding(4.f)
                [BuildRightPanel()]
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

                + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 4, 0, 2)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("FUEL")))
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                        .ColorAndOpacity(C_AccentGreen)
                ]

                + SVerticalBox::Slot().FillHeight(1.f).Padding(0, 2)
                [BuildFuelGauge()]

                + SVerticalBox::Slot().AutoHeight().Padding(0, 6)
                [
                    SNew(SBorder)
                        .BorderBackgroundColor(C_PanelBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .Padding(FMargin(0, 1))
                        [SNew(SSpacer)]
                ]

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 2)
            [
                SNew(STextBlock)
                    .Text(FText::FromString(TEXT("THROTTLE")))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                    .ColorAndOpacity(C_AccentAmber)
            ]

            // Throttle indicator — centered alone
            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 4)
            [
                BuildThrottleIndicator()
            ]

            // Camera toggle (left) + Thrust Switch (right) — evenly split
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .FillWidth(1.f)
                .HAlign(HAlign_Center)
                [
                    SNew(SBox)
                    .WidthOverride(48.f)
                    .HeightOverride(48.f)
                    [
                        BuildBottomCameraToggle()
                    ]
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)
                .HAlign(HAlign_Center)
                [
                    BuildThrustSwitch()
                ]
            ]
        ];
}

// ─── Fuel Gauge ───────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildFuelGauge()
{
    return SNew(SBox)
        .WidthOverride(40.f)
        [
            SNew(SBorder)
                .BorderBackgroundColor(C_PanelBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                .Padding(3.f)
                [
                    SNew(SOverlay)

                        + SOverlay::Slot()
                        [
                            SNew(SBorder)
                                .BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.f))
                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        ]

                        + SOverlay::Slot()
                        .VAlign(VAlign_Bottom)
                        [
                            SNew(SBox)
                                .HeightOverride_Lambda([this]() -> FOptionalSize
                                    {
                                        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                        {
                                            ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                            if (P)
                                            {
                                                float Pct = FMath::Clamp(P->Fuel / P->MaxFuel, 0.f, 1.f);
                                                return FOptionalSize(Pct * 188.f);
                                            }
                                        }
                                        return FOptionalSize(188.f);
                                    })
                                [
                                    SNew(SBorder)
                                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                        .BorderBackgroundColor_Lambda([this]() -> FLinearColor
                                            {
                                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                                {
                                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                                    if (P) return (P->Fuel / P->MaxFuel) <= 0.20f ? C_FuelLow : C_FuelFull;
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
                            ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                            if (P) return P->bIsBoosting ? C_AccentGreen : C_PanelBorder;
                        }
                        return C_PanelBorder;
                    })
                [
                    SNew(STextBlock)
                        .Text_Lambda([this]() -> FText
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P) return FText::FromString(P->bIsBoosting ? TEXT("ON") : TEXT("OFF"));
                                }
                                return FText::FromString(TEXT("OFF"));
                            })
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                        .ColorAndOpacity(C_White)
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
                        P->PlaySwitchSound();
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

                + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
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

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(0, 2)
                [
                    SNew(SBox).WidthOverride(20.f).HeightOverride(40.f)
                        [
                            SNew(SOverlay)

                                + SOverlay::Slot()
                                [
                                    SNew(SBorder)
                                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                        .BorderBackgroundColor(C_PanelBorder)
                                ]

                                + SOverlay::Slot().VAlign(VAlign_Top)
                                [
                                    SNew(SBox).HeightOverride(18.f)
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

                            + SOverlay::Slot().VAlign(VAlign_Bottom)
                                [
                                    SNew(SBox).HeightOverride(18.f)
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

            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
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

                + SVerticalBox::Slot().FillHeight(0.35f).Padding(0, 0, 0, 6)
                [BuildComputerDisplay()]

                + SVerticalBox::Slot().FillHeight(0.65f)
                [BuildKeypad()]
        ];
}

// ─── Computer Display ─────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildComputerDisplay()
{
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

                        + SVerticalBox::Slot().FillHeight(1.f).VAlign(VAlign_Center).HAlign(HAlign_Center)
                        [
                            SNew(STextBlock)
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 32))
                                .ColorAndOpacity(C_DisplayText)
                                .Text_Lambda([this]() -> FText
                                    {
                                        if (MyOwnerHUD.IsValid())
                                        {
                                            FString Q = MyOwnerHUD->CurrentQuestionText;
                                            if (Q.IsEmpty()) return FText::FromString(TEXT("PRESS W A S D"));
                                            return FText::FromString(Q);
                                        }
                                        return FText::FromString(TEXT("NO HUD"));
                                    })
                        ]

                    + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
                        [
                            SNew(SBox).WidthOverride(200.f)
                                [
                                    SAssignNew(AnswerInputBox, SEditableText)
                                        .HintText(FText::FromString(TEXT("_ _ _")))
                                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 28))
                                        .ColorAndOpacity(C_DisplayText)
                                        .IsReadOnly(true)
                                        .OnKeyDownHandler(this, &SCockpitWidget::OnAnswerCommitted)
                                ]
                        ]
                ]
        ];
}

// ─── Keypad ───────────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildKeypad()
{
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
    if (!bInputEnabled) return FReply::Handled();

    if (!MyOwnerHUD.IsValid()) return FReply::Handled();
    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
    if (!P || !P->bGameStarted) return FReply::Handled();

    if (Label == "ENTER")
    {
        if (P) P->PlayKeyClickSound();
        CheckAnswer();
        return FReply::Handled();
    }

    if (!MyOwnerHUD->bQuestionActive)
    {
        return FReply::Handled();
    }

    if (P) P->PlayKeyClickSound();

    AppendToInput(Label);
    return FReply::Handled();
}

// ─── Append To Input ──────────────────────────────────────────────
void SCockpitWidget::AppendToInput(FString Character)
{
    if (!AnswerInputBox.IsValid()) return;
    FString Current = AnswerInputBox->GetText().ToString();
    if (Current.Len() < 6)
        AnswerInputBox->SetText(FText::FromString(Current + Character));
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

                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .Padding(0, 4)
                [
                    SNew(SBox)
                        .WidthOverride(130.f)
                        .HeightOverride(130.f)
                        [
                            BuildMinimap()
                        ]
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 4)
                [BuildAltitudePanel()]

                // Mission briefing panel below altitude
                + SVerticalBox::Slot()
                .FillHeight(1.f)
                .Padding(0, 4)
                [BuildMissionPanel()]
        ];
}

// ─── Minimap ──────────────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildMinimap()
{
    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(C_PanelBorder)
        .Padding(2.f)
        [
            SNew(SMinimapWidget)
                .OwnerHUD(MyOwnerHUD)
        ];
}

// ─── Altitude Panel ───────────────────────────────────────────────
TSharedRef<SWidget> SCockpitWidget::BuildAltitudePanel()
{
    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(C_PanelBg)
        .Padding(8.f)
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot().AutoHeight()
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

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
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

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
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

TSharedRef<SWidget> SCockpitWidget::BuildMissionPanel()
{
    return SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(C_PanelBg)
        .Padding(6.f)
        [
            SNew(SVerticalBox)

                // Header: "MISSION FROM [CHARACTER NAME]"
                + SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 4)
                [
                    SNew(STextBlock)
                        .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                        .ColorAndOpacity(C_AccentGreen)
                        .Text_Lambda([this]() -> FText
                            {
                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                {
                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                    if (P && P->SelectedCharacter)
                                        return FText::FromString(
                                            FString::Printf(TEXT("MISSION FROM %s"),
                                                *P->SelectedCharacter->CharacterName.ToUpper()));
                                }
                                return FText::FromString(TEXT("MISSION"));
                            })
                ]

            // Divider
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
                [
                    SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(C_PanelBorder)
                        .Padding(FMargin(0, 1))
                        [SNew(SSpacer)]
                ]

            // Character portrait
            + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left).Padding(0, 4)
                [
                    SNew(SBox)
                        .WidthOverride(40.f)
                        .HeightOverride(40.f)
                        [
                            SNew(SBorder)
                                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                                .BorderBackgroundColor(C_PanelBorder)
                                [
                                    SNew(SImage)
                                        .Image_Lambda([this]() -> const FSlateBrush*
                                            {
                                                if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
                                                {
                                                    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
                                                    if (P && P->SelectedCharacter && P->SelectedCharacter->CharacterPortrait)
                                                    {
                                                        PortraitBrush.SetResourceObject(P->SelectedCharacter->CharacterPortrait);
                                                        PortraitBrush.ImageSize = FVector2D(40.f, 40.f);
                                                        return &PortraitBrush;
                                                    }
                                                }
                                                return FCoreStyle::Get().GetBrush("WhiteBrush");
                                            })
                                ]
                        ]
                ]

            // Typewriter mission text
            + SVerticalBox::Slot().FillHeight(1.f).Padding(0, 4)
                [
                    SNew(STextBlock)
                        .Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
                        .ColorAndOpacity(C_DisplayText)
                        .AutoWrapText(true)
                        .Text_Lambda([this]() -> FText
                            {
                                return FText::FromString(DisplayedMissionText);
                            })
                ]
        ];
}


// ─── SetInputEnabled ──────────────────────────────────────────────
void SCockpitWidget::SetInputEnabled(bool bEnabled)
{
    bInputEnabled = bEnabled;
}

// ─── OnKeyDown ────────────────────────────────────────────────────
FReply SCockpitWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (!MyOwnerHUD.IsValid()) return FReply::Unhandled();
    if (!bInputEnabled) return FReply::Handled();

    ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
    if (!P || !P->bGameStarted)
    {
        // Allow pause key even during briefing
        if (InKeyEvent.GetKey() == EKeys::Escape)
        {
            AMyHUD* HUD = Cast<AMyHUD>(MyOwnerHUD.Get());
            if (HUD)
            {
                if (HUD->bIsPaused)
                    HUD->HidePauseScreen();
                else
                    HUD->ShowPauseScreen();
            }
            return FReply::Handled();
        }
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::L)
    {
        if (P)
        {
            P->PlaySwitchSound();
            P->ToggleThrustMode();
            ResultText = FText::FromString(P->bForwardThrustMode ?
                TEXT("FORWARD THRUST") : TEXT("VERTICAL THRUST"));
            ResultColor = FSlateColor(C_AccentAmber);
        }
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::C)
    {
        if (P)
        {
            P->ToggleBottomCamera();
        }
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        if (MyOwnerHUD.IsValid())
        {
            AMyHUD* HUD = Cast<AMyHUD>(MyOwnerHUD.Get());
            if (HUD)
            {
                if (HUD->bIsPaused)
                    HUD->HidePauseScreen();
                else
                    HUD->ShowPauseScreen();
            }
        }
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::R)
    {
        if (MyOwnerHUD->bQuestionActive)
        {
            MyOwnerHUD->bQuestionActive = false;
            MyOwnerHUD->CurrentQuestionText = TEXT("");
            ResultText = FText::FromString(TEXT("CANCELLED"));
            ResultColor = FSlateColor(C_AccentAmber);

            if (AnswerInputBox.IsValid())
                AnswerInputBox->SetText(FText::GetEmpty());

            FSlateApplication::Get().SetKeyboardFocus(AsShared());
        }
        return FReply::Handled();
    }

    if (!MyOwnerHUD->bQuestionActive)
    {
        FString Type = TEXT("");
        if (InKeyEvent.GetKey() == EKeys::A) Type = "a";
        else if (InKeyEvent.GetKey() == EKeys::S) Type = "d";
        else if (InKeyEvent.GetKey() == EKeys::W) Type = "m";
        else if (InKeyEvent.GetKey() == EKeys::D) Type = "s";

        if (!Type.IsEmpty())
        {
            if (P) P->PlayKeyClickSound();

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
            FSlateApplication::Get().SetKeyboardFocus(AsShared());
            return FReply::Handled();
        }
    }

    // Number key input - only when question is active
    if (MyOwnerHUD->bQuestionActive)
    {
        FString NumStr = TEXT("");
        if (InKeyEvent.GetKey() == EKeys::Zero || InKeyEvent.GetKey() == EKeys::NumPadZero)  NumStr = "0";
        else if (InKeyEvent.GetKey() == EKeys::One || InKeyEvent.GetKey() == EKeys::NumPadOne)   NumStr = "1";
        else if (InKeyEvent.GetKey() == EKeys::Two || InKeyEvent.GetKey() == EKeys::NumPadTwo)   NumStr = "2";
        else if (InKeyEvent.GetKey() == EKeys::Three || InKeyEvent.GetKey() == EKeys::NumPadThree) NumStr = "3";
        else if (InKeyEvent.GetKey() == EKeys::Four || InKeyEvent.GetKey() == EKeys::NumPadFour)  NumStr = "4";
        else if (InKeyEvent.GetKey() == EKeys::Five || InKeyEvent.GetKey() == EKeys::NumPadFive)  NumStr = "5";
        else if (InKeyEvent.GetKey() == EKeys::Six || InKeyEvent.GetKey() == EKeys::NumPadSix)   NumStr = "6";
        else if (InKeyEvent.GetKey() == EKeys::Seven || InKeyEvent.GetKey() == EKeys::NumPadSeven) NumStr = "7";
        else if (InKeyEvent.GetKey() == EKeys::Eight || InKeyEvent.GetKey() == EKeys::NumPadEight) NumStr = "8";
        else if (InKeyEvent.GetKey() == EKeys::Nine || InKeyEvent.GetKey() == EKeys::NumPadNine)  NumStr = "9";
        else if (InKeyEvent.GetKey() == EKeys::Enter)
        {
            if (P) P->PlayKeyClickSound();
            CheckAnswer();
            return FReply::Handled();
        }
        else if (InKeyEvent.GetKey() == EKeys::BackSpace)
        {
            if (AnswerInputBox.IsValid())
            {
                FString Current = AnswerInputBox->GetText().ToString();
                if (Current.Len() > 0)
                    AnswerInputBox->SetText(FText::FromString(Current.LeftChop(1)));
            }
            return FReply::Handled();
        }

        if (!NumStr.IsEmpty())
        {
            if (P) P->PlayKeyClickSound();
            AppendToInput(NumStr);
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
    int32   PlayerAnswer = FCString::Atoi(*InputStr);
    bool bCorrect = (PlayerAnswer == MyOwnerHUD->CurrentCorrectAnswer);

    if (PlayerAnswer == MyOwnerHUD->CurrentCorrectAnswer)
    {
        ResultText = FText::FromString(TEXT("CORRECT!"));
        if (MyOwnerHUD.IsValid())
        {
            MyOwnerHUD->TotalQuestionsAnswered++;
            MyOwnerHUD->TotalQuestionsCorrect++;
        }
        ResultColor = FSlateColor(C_AccentGreen);
        MyOwnerHUD->bQuestionActive = false;
        MyOwnerHUD->CurrentQuestionText = TEXT("");

        ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
        if (P)
        {
            P->PlayCorrectAnswerSound();

            if (MyOwnerHUD->QuestionType == "d")
            {
                P->AddFuel(MyOwnerHUD->FuelRewardAmount);
                P->PlayFuelSound();
                ResultText = FText::FromString(
                    FString::Printf(TEXT("CORRECT! +%.0f FUEL"), MyOwnerHUD->FuelRewardAmount));
            }
            else if (MyOwnerHUD->QuestionType == "m")
            {
                if (P->Fuel <= 0.0f)
                {
                    ResultText = FText::FromString(TEXT("NO FUEL!"));
                    ResultColor = FSlateColor(C_FuelLow);
                }
                else
                {
                    P->ActivateBoost();
                    ResultText = FText::FromString(P->bForwardThrustMode ?
                        TEXT("CORRECT! FORWARD BOOST!") : TEXT("CORRECT! BOOST!"));
                }
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
        if (MyOwnerHUD.IsValid())
        {
            MyOwnerHUD->TotalQuestionsAnswered++;
        }
        ResultColor = FSlateColor(C_FuelLow);

        ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());  
        if (P) P->PlayWrongAnswerSound();
    }

    AnswerInputBox->SetText(FText::GetEmpty());
    FSlateApplication::Get().SetKeyboardFocus(AsShared());
}

void SCockpitWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    // Initialize typewriter text from the selected character
    if (!bTypewriterActive && FullMissionText.IsEmpty())
    {
        if (MyOwnerHUD.IsValid() && MyOwnerHUD->GetOwningPawn())
        {
            ALanderPawn* P = Cast<ALanderPawn>(MyOwnerHUD->GetOwningPawn());
            if (P && P->SelectedCharacter)
            {
                FullMissionText = P->SelectedCharacter->MissionDialogue.ToString();
                bTypewriterActive = true;
                TypewriterIndex = 0;
                DisplayedMissionText = TEXT("");
            }
        }
    }

    // Don't advance typewriter while paused
    if (MyOwnerHUD.IsValid())
    {
        AMyHUD* HUD = Cast<AMyHUD>(MyOwnerHUD.Get());
        if (HUD && HUD->bIsPaused) return;
    }

    // Typewriter logic
    if (bTypewriterActive && TypewriterIndex < FullMissionText.Len())
    {
        float CurrentCharsPerSecond = CharsPerSecond;
        if (TypewriterIndex > 0)
        {
            TCHAR PrevChar = FullMissionText[TypewriterIndex - 1];
            if (PrevChar == '.' || PrevChar == '!' || PrevChar == '?')
            {
                CurrentCharsPerSecond = CharsPerSecond * 0.2f;
            }
        }

        TypewriterAccumulator += InDeltaTime;
        float TimePerChar = 1.0f / CurrentCharsPerSecond;

        while (TypewriterAccumulator >= TimePerChar && TypewriterIndex < FullMissionText.Len())
        {
            DisplayedMissionText += FullMissionText[TypewriterIndex];
            TypewriterIndex++;
            TypewriterAccumulator -= TimePerChar;
        }

        if (TypewriterIndex >= FullMissionText.Len())
        {
            bTypewriterActive = false;
        }
    }
}