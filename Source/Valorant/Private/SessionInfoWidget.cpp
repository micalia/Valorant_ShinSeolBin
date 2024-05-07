// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfoWidget.h"
#include "NetGameInstance.h"
#include "Components/Button.h"


void USessionInfoWidget::NativeConstruct()
{
	gi = GetGameInstance<UNetGameInstance>();

	btn_Join->OnClicked.AddDynamic(this, &USessionInfoWidget::OnClickedJoinButton);
}

void USessionInfoWidget::OnClickedJoinButton()
{
	if (gi != nullptr)
	{
		gi->JoinSelectedSession(sessionIndex);
	}
}
