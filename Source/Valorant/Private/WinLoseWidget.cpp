// Fill out your copyright notice in the Description page of Project Settings.


#include "WinLoseWidget.h"
#include "Components/Overlay.h"
#include "Animation/WidgetAnimation.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include <Sound/SoundBase.h>
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

	AnimFinshed.BindDynamic(this, &UWinLoseWidget::EndWinAnim);
	BindToAnimationFinished(WinBackGroundAnim, AnimFinshed);
	
}

void UWinLoseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UWinLoseWidget::StartLoseAnim() // 졌을때 위에 나오는 UI
{
	OV_Loss->SetVisibility(ESlateVisibility::Visible);  // 

	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("Loooss")), true, FVector2D(1, 1));


	//ServerPlayLoseAnim();
	PlayAnimation(LossAnimation); // 애니메이션 실행

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay,FTimerDelegate::CreateLambda([&](){
		OV_LoseBack->SetVisibility(ESlateVisibility::Visible); StartLoseBackAnim();
	}),3.0f,false);

}

void UWinLoseWidget::onDestory()
{
	this->RemoveFromParent();
}

void UWinLoseWidget::EndWinAnim()
{
	//WinBackGroundAnim->
	////GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, FString::Printf(TEXT("EndWinAnim")), true, FVector2D(1, 1));

}

void UWinLoseWidget::StartWinAnim() // 이겼을때 위에 나오는 UI
{
	//soundV = LoadObject<USoundBase>(nullptr, TEXT("/Game/LMH/Sounds/09-Victory.09-Victory"));
	/*if (soundV) {
		UGameplayStatics::PlaySound2D(GetWorld(),soundV);

	}*/
	OV_Win->SetVisibility(ESlateVisibility::Visible);

	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("Wiiin")), true, FVector2D(1, 1));


	PlayAnimation(WinAnimation);
	//ServerPlayWinAnim();

	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		OV_WinBack->SetVisibility(ESlateVisibility::Visible);
	StartWinBackAnim();
		}), 3.0f, false);
}
//
//void UWinLoseWidget::ServerPlayWinAnim_Implementation()
//{
//	MultiPlayWinAnim_Implementation();
//}
//
//void UWinLoseWidget::ServerPlayLoseAnim_Implementation()
//{
//	MultiPlayLoseAnim_Implementation();
//}
//
//void UWinLoseWidget::MultiPlayWinAnim_Implementation()
//{
//	PlayAnimation(WinAnimation); // 애니메이션 실행
//}
//
//void UWinLoseWidget::MultiPlayLoseAnim_Implementation()
//{
//	PlayAnimation(LossAnimation); // 애니메이션 실행
//}



void UWinLoseWidget::StartLoseBackAnim() // 졌을떄 백그라운드 UI 
{
	PlayAnimationForward(LoseBackGroundAnim);
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		PlayAnimationReverse(LoseBackGroundAnim);
		}), 3.0f, false);
	
	/*OV_LoseBack->SetVisibility(ESlateVisibility::Hidden);*/
}

void UWinLoseWidget::StartWinBackAnim() //이겼을떄 백그라운드 UI 
{
	PlayAnimationForward(WinBackGroundAnim);
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]() {
		PlayAnimationReverse(WinBackGroundAnim);
	OV_WinBack->SetVisibility(ESlateVisibility::Hidden);
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

