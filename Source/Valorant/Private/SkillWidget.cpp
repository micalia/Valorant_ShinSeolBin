// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "PlayerFireComponent.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include "SB_Sova.h"
#include "InGameTopUi.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (MyPlayerController) {
		me = Cast<ABaseCharacter>(MyPlayerController->GetPawn());
	}
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	

	if (WB_TopUI && me) {
		auto GameStateVal = GetWorld()->GetGameState();
		if (GameStateVal) {
			TArray<APlayerState*> players = GameStateVal->PlayerArray;
			FString playersInfoText;

			for (APlayerState* p : players)
			{
				auto a = MyPlayerController->GetPawn();
				if (a) {
					if (p != MyPlayerController->GetPawn()->GetPlayerState()) {
						WB_TopUI->EnemyName_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *p->GetPlayerName())));
						WB_TopUI->EnemyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)p->GetScore())));
					}
				}
			}
		}


		// 플레이어의 총알 정보 갱신
		if (me->GetPlayerState()) {
			WB_TopUI->MyName_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *me->GetPlayerState()->GetPlayerName())));
			WB_TopUI->MyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)me->GetPlayerState()->GetScore())));
		}

		if (Hp_txt) {
			Hp_txt->SetText(FText::AsNumber(me->GetHP()));
		}
	}
}