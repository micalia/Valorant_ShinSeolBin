// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ANetGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	bool endGame;
	
	float DelayTime = 1;
	float currTime = 0;

	virtual void Tick(float DeltaTime) override;

	TArray<class APlayerState*> GetPlayerArrayByScore();

	UPROPERTY()
	TArray<AActor*> playerCountArr;

	UPROPERTY(BlueprintReadWrite)
	int32 playerCount = 0;
};
