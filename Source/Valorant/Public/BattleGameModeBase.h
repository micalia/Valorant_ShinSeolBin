// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ABattleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABattleGameModeBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APlayerStart>> startArray;

//UPROPERTY(EditAnywhere)
//TSubclassOf<class UWinLoseWidget> WinLoseWidgetFactory;
//	UPROPERTY()
//	class UWinLoseWidget* winloseInstance;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	int32 callNumber = 0;

	UPROPERTY()
	TArray<AActor*> playerCountArr;
	float DelayTime = 1;
	float currTime = 0;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 playerCount = 0;
};
