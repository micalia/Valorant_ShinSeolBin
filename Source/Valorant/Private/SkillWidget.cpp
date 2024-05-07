// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "PlayerFireComponent.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include "SB_Sova.h"

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
	//���� Pawn�� Score�� �������� �ʹ�.
	// ���� me�� PlayerState�� ������ �� �̸��� ����Ѵ�.
	//me->GetPlayerState()->GetPlayerName();
	//������ܿ� ������ �� �̸��� ����, �������� ��� �̸��� ������ �ְ� �ʹ�.
	
	//if () {
		auto GameStateVal = GetWorld()->GetGameState();
		if (GameStateVal) {
			TArray<APlayerState*> players = GameStateVal->PlayerArray;
			FString playersInfoText;

			for (APlayerState* p : players)
			{
				auto a = MyPlayerController->GetPawn();
				if (a) {

					if (p != MyPlayerController->GetPawn()->GetPlayerState()) {
						EnemyNameAndScore_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *p->GetPlayerName())));
						EnemyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)p->GetScore())));
					}
					else {
						//playersInfoText.Append(FString::Printf(TEXT("%s: %d\n"), *p->GetPlayerName(), (int32)p->GetScore()));
					}
				}
			}
		}

			
	// �÷��̾��� �Ѿ� ���� ����
	if (me) {
		if (MyNameAndScore_txt) {
			if (me->GetPlayerState()) {
				MyNameAndScore_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *me->GetPlayerState()->GetPlayerName())));
				MyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)me->GetPlayerState()->GetScore())));
			}
		}

		if (Hp_txt) {
			Hp_txt->SetText(FText::AsNumber(me->GetHP()));
		}
	}
}