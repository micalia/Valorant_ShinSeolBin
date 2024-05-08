// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_AirSmokeMarker.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API USB_AirSmokeMarker : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	class UImage* AirSmokeMarker;
	UFUNCTION()
	void RemoveThisUI();
};
