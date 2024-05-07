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
	//현재 Pawn의 Score를 가져오고 싶다.
	// 현재 me의 PlayerState를 가져온 후 이름을 출력한다.
	//me->GetPlayerState()->GetPlayerName();
	//위젯상단에 왼쪽은 내 이름과 점수, 오른쪽은 상대 이름과 점수를 넣고 싶다.
	
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

			
	// 플레이어의 총알 정보 갱신
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