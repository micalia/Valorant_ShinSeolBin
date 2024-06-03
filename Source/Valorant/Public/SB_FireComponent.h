// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "SB_FireComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API USB_FireComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	USB_FireComponent();

	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;
};
