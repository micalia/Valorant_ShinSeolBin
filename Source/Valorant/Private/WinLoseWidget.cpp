// Fill out your copyright notice in the Description page of Project Settings.


#include "WinLoseWidget.h"
#include "Components/Overlay.h"
#include "Animation/WidgetAnimation.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include <Sound/SoundBase.h>
#include "Animation/UMGSequencePlayer.h"
#include "../../Engine/Classes/GameFramework/PlayerState.h"
#include "../../UMG/Public/Components/TextBlock.h"

void UWinLoseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OV_standBy->SetVisibility(ESlateVisibility::Hidden);
	OV_Loss->SetVisibility(ESlateVisibility::Hidden);
	OV_Win->SetVisibility(ESlateVisibility::Hidden);
	OV_WinBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseEnding->SetVisibility(ESlateVisibility::Hidden);
	OV_WinEnding->SetVisibility(ESlateVisibility::Hidden);

	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (MyPlayerController) {
		me = Cast<ABaseCharacter>(MyPlayerController->GetPawn());
	}

	AnimFinshed.BindDynamic(this, &UWinLoseWidget::onDestory);
	BindToAnimationFinished(LossAnimation, AnimFinshed);
	GetWorld()->GetTimerManager().ClearTimer(SetWinLoseNickDelay);
	GetWorld()->GetTimerManager().SetTimer(SetWinLoseNickDelay, FTimerDelegate::CreateLambda([&]() {
		APlayerController* MyCon = UGameplayStatics::GetPlayerController(this, 0);
		MyNick = MyCon->GetPawn()->GetPlayerState()->GetPlayerName();
		WinnerNick->SetText(FText::FromString(MyNick));
		LoserNick->SetText(FText::FromString(MyNick));
	}), 1.0f, false);
}

void UWinLoseWidget::StartLoseAnim() // 졌을때 위에 나오는 UI
{
	OV_Loss->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(LossAnimation); // 애니메이션 실행

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay,FTimerDelegate::CreateLambda([&](){
		OV_LoseBack->SetVisibility(ESlateVisibility::Visible); 
		StartLoseBackAnim();
	}),3.0f,false);

}

void UWinLoseWidget::onDestory()
{
	this->RemoveFromParent();
}

void UWinLoseWidget::StartWinAnim() // 이겼을때 위에 나오는 UI
{
	OV_Win->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(WinAnimation);

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		OV_WinBack->SetVisibility(ESlateVisibility::Visible);
	StartWinBackAnim();
		}), 3.0f, false);
}

void UWinLoseWidget::StartLoseBackAnim() // 졌을떄 백그라운드 UI 
{
	PlayAnimationForward(LoseBackGroundAnim);
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		PlayAnimationReverse(LoseBackGroundAnim);
		}), 3.0f, false);
}

void UWinLoseWidget::StartWinBackAnim() //이겼을떄 백그라운드 UI 
{
	PlayAnimationForward(WinBackGroundAnim);
	
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		SequencePlayer = PlayAnimationReverse(WinBackGroundAnim);
		OV_WinBack->SetVisibility(ESlateVisibility::Hidden);
		SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &UWinLoseWidget::OnSequenceFinished);
		}), 3.0f, false);
}

void UWinLoseWidget::StartWinEndingAnim()
{
	OV_WinEnding->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimationForward(WinEndingAnim);
}

void UWinLoseWidget::StartLoseEndingAnim()
{
	OV_LoseEnding->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimationForward(LoseEndingAnim); 
}

void UWinLoseWidget::OnSequenceFinished(class UUMGSequencePlayer& InSequencePlayer)
{
	OV_standBy->SetVisibility(ESlateVisibility::Hidden);
	OV_Loss->SetVisibility(ESlateVisibility::Hidden);
	OV_Win->SetVisibility(ESlateVisibility::Hidden);
	OV_WinBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseEnding->SetVisibility(ESlateVisibility::Hidden);
	OV_WinEnding->SetVisibility(ESlateVisibility::Hidden);
}
