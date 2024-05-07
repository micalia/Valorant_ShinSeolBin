// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"
#include "NetGameInstance.h"

void ANetPlayerState::BeginPlay() {
	Super::BeginPlay();

	gi = Cast<UNetGameInstance>(GetGameInstance());

	if (gi != nullptr && GetPlayerController() != nullptr && GetPlayerController()->IsLocalPlayerController()) {
		SetMyName(gi->mySessionName);
	}
}

void ANetPlayerState::SetMyName_Implementation(const FString& myName)
{
	SetPlayerName(myName);
}
