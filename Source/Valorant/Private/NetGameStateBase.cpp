// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"


void ANetGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DelayTime;

	if (DelayTime < currTime) {
		playerCountArr.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), playerCountArr);
		playerCount = playerCountArr.Num();
		currTime = 0;
	}
}

TArray<APlayerState*> ANetGameStateBase::GetPlayerArrayByScore()
{
	TArray<APlayerState*> sortedPlayers = PlayerArray;

	sortedPlayers.Sort([](const APlayerState& p1, const APlayerState& p2) { return p1.GetScore() > p2.GetScore(); });
	return sortedPlayers;
}
