// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "BattleGameModeBase.h"
#include <GameFramework/SpectatorPawn.h>
#include "SB_Sova.h"

void ANetPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		gm = GetWorld()->GetAuthGameMode<ABattleGameModeBase>();
	}
}

// 재시작 함수
void ANetPlayerController::ServerRespawnPlayer_Implementation()
{
	if (gm != nullptr)
	{
		APawn* playerPawn = GetPawn();
		UnPossess();
		playerPawn->Destroy();

		// 처음 배정받은 PlayerStart 액터 위치에서 리스폰
		//if (HasAuthority()) {
			gm->RestartPlayer(this);
		//}

		// 지정된 Transform에서 리스폰
	/*	FTransform restartPoint;
		restartPoint.SetLocation(FVector(7880.000000,  11294.614303,  192.000005));*/

		/*gm->RestartPlayerAtTransform(this, restartPoint);*/
	}
}



void ANetPlayerController::ServerChangePlayerToSpectator_Implementation()
{
	// 플레이어로부터 Unpossess를 한다.

	UE_LOG(LogTemp, Warning, TEXT("ServerChange NetMode : %d"), GetNetMode());

	APawn* player = GetPawn();
	
	if (player) {
		UnPossess();
	}

	// 관전자 폰을 생성한다.
	if (gm != nullptr)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetTransform(), param);

		if (spectator != nullptr)
		{
			// 생성된 관전자 폰에 빙의한다.
			SetShowMouseCursor(false);
			Possess(spectator);
		}
	}
	// 플레이어를 제거한다.
	player->Destroy();

		// 5초 뒤에 리스폰한다.
		FTimerHandle respawnHandle;

		GetWorldTimerManager().SetTimer(respawnHandle, this, &ANetPlayerController::ServerRespawnPlayer_Implementation, 1.0f, false);

}
