// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_Weapon_inv;
};
