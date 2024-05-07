// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeOut.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UFadeOut : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation * Fadeout;

public:
	void fadIn();

	UFUNCTION()
	void OnRestart();

	UFUNCTION()
	void onDestory();

	class ABattleGameModeBase* BM;
	//UFUNCTION()
	//void OnAnimationFinished();

	FWidgetAnimationDynamicEvent AnimFinshed;
private:
	class ABaseCharacter* player;

/*	UFUNCTION()
		void OnAnimationFinished()*/;

};
