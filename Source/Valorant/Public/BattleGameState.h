// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BattleGameState.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ABattleGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABattleGameState();

	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere)
TSubclassOf<class UWinLoseWidget> WinLoseWidgetFactory;
	UPROPERTY()
	class UWinLoseWidget* winloseInstance;*/
};
