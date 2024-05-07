// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SB_ScoutingArrow.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UUI_SB_ScoutingArrow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PowerGaugeBar;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* BounceCount1_img;

	UPROPERTY(meta = (BindWidget))
	class UImage* BounceCount2_img;

	//프로토
	FLinearColor NotActiveColor = FLinearColor(0, 0, 0, 1);
	FLinearColor ActiveColor = FLinearColor(1, 1, 1, 1);

	//정찰용 화살 충전 게이지 퍼센트 색깔
	FLinearColor LackGaugeColor = FLinearColor(0.093059, 0.686685, 0.47932, 1);
	FLinearColor FullGaugeColor = FLinearColor(1, 0.000911, 0.000607, 1);
};
