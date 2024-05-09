// Fill out your copyright notice in the Description page of Project Settings.


#include "AirSmokeMinimapWidget.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/SB_Sova.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "../Public/SB_AirSmokeMarker.h"

UAirSmokeMinimapWidget::UAirSmokeMinimapWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundBase> tempSovaMinimapClickSound(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/AirSmoke/Sky-Smoke-Select.Sky-Smoke-Select'"));
	if (tempSovaMinimapClickSound.Succeeded())
	{
		SovaMinimapClickSound = tempSovaMinimapClickSound.Object;
	}
	ConstructorHelpers::FClassFinder<USB_AirSmokeMarker> tempAirSmokeMarkerFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_AirSmokeMarker.WB_AirSmokeMarker_C'"));
	if (tempAirSmokeMarkerFactory.Succeeded()) {
		AirSmokeMarkerFactory = tempAirSmokeMarkerFactory.Class;
	}
}

FReply UAirSmokeMinimapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		if (auto MyPawn = GetWorld()->GetFirstPlayerController()->GetPawn()) {
			if (auto Sova = Cast<ASB_Sova>(MyPawn)) {
				if (Sova->airSmokeMaxCount > Sova->airSmokeCurrCount) {
					Sova->AirSmokeMarkerRef = CreateWidget<USB_AirSmokeMarker>(GetWorld(), AirSmokeMarkerFactory);
					if (Sova->AirSmokeMarkerRef) {
						Sova->AirSmokeMarkerRef->AddToViewport();
					}
					Sova->airSmokeCurrCount++;
					UGameplayStatics::PlaySound2D(GetWorld(), SovaMinimapClickSound);
					if (APlayerController* Player = GetOwningPlayer()) {
						UWidgetLayoutLibrary::GetMousePositionScaledByDPI(Player, LocationX, LocationY);
						float ViewportScaleRatio = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
						float ScaleFitMouseX = LocationX * ViewportScaleRatio;
						float ScaleFitMouseY = LocationY * ViewportScaleRatio;

						FVector2D CurrViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
						float SubtractXsize = CurrViewportSize.X / 2;
						float SubtractYsize = CurrViewportSize.Y / 2;

						float SubtractResultX = ScaleFitMouseX - SubtractXsize;
						float SubtractResultY = ScaleFitMouseY - SubtractYsize;

						FVector2D MouseCoordinate = FVector2D(SubtractResultY, SubtractResultX) * FVector2D(1, -1);
						
						if (auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Cast<UWidget>(SmokeSkillMiniMap))) {
							FVector2D CanvasSize = CanvasSlot->GetSize();
							FVector2D ScaleFitCanvasSize = FVector2D(CanvasSize.X * ViewportScaleRatio, CanvasSize.Y * ViewportScaleRatio);
							FVector2D AirSmokeSpawnPos = MouseCoordinate * (WorldScale / ScaleFitCanvasSize);
							SpawnSmokePos.Add(FVector(AirSmokeSpawnPos.X, AirSmokeSpawnPos.Y, 0));
						}
					}
				}
			}
		}
	}
	return FReply::Unhandled();
}