// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	//
public:
	UPROPERTY(EditAnywhere, Category = "MySettings | Widget")
	TSubclassOf<class UMinimapWidget> minimapWidget;

	UPROPERTY()
	class UMinimapWidget* minimap;

	UPROPERTY(EditAnywhere, Category = "MySettings | Widget")
	TSubclassOf<class USkillWidget> skillWidget;

	UPROPERTY()
	class USkillWidget* skil;

	UPROPERTY(EditAnywhere, Category = "MySettings | Widget")
	TSubclassOf<class UVSWidget> vsWidget;

	UPROPERTY()
	class UVSWidget* vs;

	UPROPERTY(EditAnywhere, Category = "MySettings | Widget")
	TSubclassOf<class UInventoryWidget> invWidget;

	UPROPERTY()
	class UInventoryWidget* inv;

};
