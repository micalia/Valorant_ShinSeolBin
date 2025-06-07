// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "BattleGameModeBase.h"
#include <GameFramework/SpectatorPawn.h>

void ANetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		gm = GetWorld()->GetAuthGameMode<ABattleGameModeBase>();
	}
}

// ����� �Լ�
void ANetPlayerController::ServerRespawnPlayer_Implementation()
{
	if (gm != nullptr)
	{
		APawn* playerPawn = GetPawn();
		UnPossess();
		playerPawn->Destroy();

		// ó�� �������� PlayerStart ���� ��ġ���� ������
		//if (HasAuthority()) {
			gm->RestartPlayer(this);
		//}

		// ������ Transform���� ������
	/*	FTransform restartPoint;
		restartPoint.SetLocation(FVector(7880.000000,  11294.614303,  192.000005));*/

		/*gm->RestartPlayerAtTransform(this, restartPoint);*/
	}
}



void ANetPlayerController::ServerChangePlayerToSpectator_Implementation()
{
	// �÷��̾�κ��� Unpossess�� �Ѵ�.

	UE_LOG(LogTemp, Warning, TEXT("ServerChange NetMode : %d"), GetNetMode());

	APawn* player = GetPawn();
	
	if (player) {
		UnPossess();
	}

	// ������ ���� �����Ѵ�.
	if (gm != nullptr)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetTransform(), param);

		if (spectator != nullptr)
		{
			// ������ ������ ���� �����Ѵ�.
			SetShowMouseCursor(false);
			Possess(spectator);
		}
	}
	// �÷��̾ �����Ѵ�.
	player->Destroy();

		// 5�� �ڿ� �������Ѵ�.
		FTimerHandle respawnHandle;

		GetWorldTimerManager().SetTimer(respawnHandle, this, &ANetPlayerController::ServerRespawnPlayer_Implementation, 1.0f, false);

}
