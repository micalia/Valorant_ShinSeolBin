// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectionrWidget.h"
#include "Components/Button.h"
#include "../Public/NetGameInstance.h"
#include "Components/CircularThrobber.h"

void UCharacterSelectionrWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bt_1Player->OnClicked.AddDynamic(this,&UCharacterSelectionrWidget::Selection1Player);
	bt_2Player->OnClicked.AddDynamic(this,&UCharacterSelectionrWidget::Selection2Player);
	bt_3Player->OnClicked.AddDynamic(this,&UCharacterSelectionrWidget::Selection3Player);


	//SB
	gi = GetGameInstance<UNetGameInstance>();

	bt_Selection->OnClicked.AddDynamic(this, &UCharacterSelectionrWidget::EnterInGame);
}

void UCharacterSelectionrWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

}

void UCharacterSelectionrWidget::Selection1Player()
{
	PlayAnimationForward(SelectionPlayer1Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSelectionrWidget::Selection2Player()
{
	PlayAnimationForward(SelectionPlayer2Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSelectionrWidget::Selection3Player()
{
	PlayAnimationForward(SelectionPlayer3Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSelectionrWidget::EnterInGame()
{
	if (!bStartChk) { // �ѹ��� �α��� Ŭ���� �� �ְ� ó��
		bStartChk = true; 
		LoadingIcon->SetRenderOpacity(1);
	}
}

