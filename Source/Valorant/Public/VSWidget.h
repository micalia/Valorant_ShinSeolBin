// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VSWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UVSWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_teamStatus_green;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_teamStatus_red;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_topline;
};
