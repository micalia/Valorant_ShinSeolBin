// Fill out your copyright notice in the Description page of Project Settings.


#include "WinLoseWidget.h"
#include "Components/Overlay.h"
#include "Animation/WidgetAnimation.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include <Sound/SoundBase.h>
#include "Animation/UMGSequencePlayer.h"
#include "../../UMG/Public/Components/TextBlock.h"
#include "SB_Sova.h"
#include "SkillWidget.h"
#include "InGameTopUi.h"

void UWinLoseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
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
}

void UWinLoseWidget::StartLoseAnim() // ������ ���� ������ UI
{
	SetVisibility(ESlateVisibility::Visible);
	OV_Loss->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(LossAnimation); // �ִϸ��̼� ����

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		OV_LoseBack->SetVisibility(ESlateVisibility::Visible);
		StartLoseBackAnim();
	}), 3.0f, false);
}

void UWinLoseWidget::onDestory()
{
	this->RemoveFromParent();
}

void UWinLoseWidget::StartWinAnim() // �̰����� ���� ������ UI
{
	SetVisibility(ESlateVisibility::Visible);
	OV_Win->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(WinAnimation);

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		OV_WinBack->SetVisibility(ESlateVisibility::Visible);
		StartWinBackAnim();
		}), 3.0f, false);
}

void UWinLoseWidget::StartLoseBackAnim() // ������ ��׶��� UI 
{
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimationForward(LoseBackGroundAnim);
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		SequencePlayer = PlayAnimationReverse(LoseBackGroundAnim);
		SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &UWinLoseWidget::OnSequenceFinished);
		}), 3.0f, false);
}

void UWinLoseWidget::StartWinBackAnim() //�̰����� ��׶��� UI 
{
	SetVisibility(ESlateVisibility::Visible);
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
	SetVisibility(ESlateVisibility::Visible);
	ASB_Sova* MySova = Cast<ASB_Sova>(me);
	WinnerNick->SetText(MySova->skillWigetInstance->WB_TopUI->MyName_txt->GetText());
	OV_WinEnding->SetVisibility(ESlateVisibility::Visible);
	SequencePlayer = PlayAnimationForward(WinEndingAnim);
}

void UWinLoseWidget::StartLoseEndingAnim()
{
	SetVisibility(ESlateVisibility::Visible);
	ASB_Sova* MySova = Cast<ASB_Sova>(me);
	WinnerEnemyNick->SetText(MySova->skillWigetInstance->WB_TopUI->EnemyName_txt->GetText());
	OV_LoseEnding->SetVisibility(ESlateVisibility::Visible);
	SequencePlayer = PlayAnimationForward(LoseEndingAnim);
}

void UWinLoseWidget::OnSequenceFinished(class UUMGSequencePlayer& InSequencePlayer)
{
	SetVisibility(ESlateVisibility::Hidden);
	OV_standBy->SetVisibility(ESlateVisibility::Hidden);
	OV_Loss->SetVisibility(ESlateVisibility::Hidden);
	OV_Win->SetVisibility(ESlateVisibility::Hidden);
	OV_WinBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseBack->SetVisibility(ESlateVisibility::Hidden);
	OV_LoseEnding->SetVisibility(ESlateVisibility::Hidden);
	OV_WinEnding->SetVisibility(ESlateVisibility::Hidden);
}
