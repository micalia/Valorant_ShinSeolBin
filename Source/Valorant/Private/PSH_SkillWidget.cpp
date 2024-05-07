// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_SkillWidget.h"
#include "Components/ProgressBar.h"
#include "SH_Neon.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>

void UPSH_SkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	player = Cast<ASH_Neon>(GetOwningPlayerPawn());
}

void UPSH_SkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (player != nullptr)
	{
		// 소유 폰의 체력 값 반영(현재 체력 / 최대 체력)
		pb_EnegeBar->SetPercent(player->GetEnege() / player->Maxenergy);
	}
	
	TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
	FString playersInfoText;
	
	for (APlayerState* p : players)
	{
		if (p != MyPlayerController->GetPawn()->GetPlayerState()) {
			EnemyNameAndScore_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *p->GetPlayerName())));
			EnemyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)p->GetScore())));
		}
		else {
			//playersInfoText.Append(FString::Printf(TEXT("%s: %d\n"), *p->GetPlayerName(), (int32)p->GetScore()));
		}
	}

	// 플레이어의 총알 정보 갱신
	if (player) {
		if (MyNameAndScore_txt) {
			if (player->GetPlayerState()) {
				MyNameAndScore_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *player->GetPlayerState()->GetPlayerName())));
				MyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)player->GetPlayerState()->GetScore())));
			}
		}

		if (Hp_txt) {
			Hp_txt->SetText(FText::AsNumber(player->GetHP()));
		}
	}
}
