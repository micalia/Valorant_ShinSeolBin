// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeOut.h"
#include "NetPlayerController.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include "BattleGameModeBase.h"

void UFadeOut::NativeConstruct()
{
	Super::NativeConstruct();
	player = GetOwningPlayerPawn<ABaseCharacter>();
	
	AnimFinshed.BindDynamic(this, &UFadeOut::onDestory);
	BindToAnimationFinished(Fadeout, AnimFinshed);
	//fadin animation이 끝나면,,

	fadIn();

	//BM = GetWorld()->GetAuthGameMode<ABattleGameModeBase>();
	//BindToAnimationFinished(Fadeout,)
	/*btn_Retry->OnClicked.AddDynamic(this, &UBattleWidget::OnClickedRetry);*/
}

void UFadeOut::fadIn()
{
	PlayAnimationForward(Fadeout);
}

void UFadeOut::OnRestart()
{
	if (ANetPlayerController* pc = player->GetController<ANetPlayerController>())
	{
		pc->ServerChangePlayerToSpectator();
		pc->SetShowMouseCursor(false);

		FInputModeGameOnly inputMode = FInputModeGameOnly();
		pc->SetInputMode(inputMode);

		if (player)
		{
			player->GetCharacterMovement()->Activate();
			this->RemoveFromParent();
		}	
	}
}

void UFadeOut::onDestory()
{

	////GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, TEXT("Destroy!!"), true, FVector2D(1, 1));
	//UE_LOG(	LogTemp,Warning,TEXT("onDestory!!!!!!!!!!!!!11"));

	//원래 카메라로 돌아옴
	if (player) {
		player->MeshVisible();

	}

	if (player && player->HasAuthority())
	{
		//승부가 나지 않으면 다시 플레이어 스폰
		if (!player->TheEndGame) {
			player->RestartProcess();
		}
	}
	
	//Destroy
	this->RemoveFromParent();
}

