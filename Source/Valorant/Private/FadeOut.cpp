// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeOut.h"
#include "NetPlayerController.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
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

