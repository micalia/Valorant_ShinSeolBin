// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ANetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class ABattleGameModeBase* gm;

	UFUNCTION(Server, Reliable)
	void ServerRespawnPlayer();

	UFUNCTION(Server, Reliable)
	void ServerChangePlayerToSpectator();


};
