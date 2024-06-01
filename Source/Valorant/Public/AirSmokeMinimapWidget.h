// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AirSmokeMinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UAirSmokeMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	UAirSmokeMinimapWidget(const FObjectInitializer &ObjectInitializer);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY()
	class USoundBase* SovaMinimapClickSound;
public:
	UPROPERTY()
	TArray<FVector> SpawnSmokePos;
	UPROPERTY(meta = (BindWidget))
	class UImage* SmokeSkillMiniMap;
	UPROPERTY(EditAnywhere)
	FVector2D WorldScale = FVector2D(15780);
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USB_AirSmokeMarker> AirSmokeMarkerFactory;
private:
	float LocationX;
	float LocationY;
};
