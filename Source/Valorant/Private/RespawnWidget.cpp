// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnWidget.h"
#include "BaseCharacter.h"
#include "Components/Button.h"
#include <Async/TaskTrace.h>

void URespawnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	player = GetOwningPlayerPawn<ABaseCharacter>();
	btn_Retry->OnClicked.AddDynamic(this, &URespawnWidget::OnClickedRetry);

}

void URespawnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URespawnWidget::OnClickedRetry()
{
	this->RemoveFromParent();
}
