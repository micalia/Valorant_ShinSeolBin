// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FireHUD.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VALORANT_API AFireHUD : public AHUD
{
	GENERATED_BODY()

	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};
