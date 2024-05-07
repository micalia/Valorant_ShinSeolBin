// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameState.h"
#include "WinLoseWidget.h"

ABattleGameState::ABattleGameState() {
	/*static ConstructorHelpers::FClassFinder<UWinLoseWidget> tempWinLoseWidgetFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WB_winLose.WB_winLose_C'"));
	if (tempWinLoseWidgetFactory.Succeeded()) {
		WinLoseWidgetFactory = tempWinLoseWidgetFactory.Class;
	}*/
}

void ABattleGameState::BeginPlay()
{
	Super::BeginPlay();
	
}
