// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLoginWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "NetGameInstance.h"
#include <UMG/Public/Components/EditableText.h>
#include <UMG/Public/Components/Slider.h>
#include "SessionInfoWidget.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

UStartLoginWidget::UStartLoginWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<USessionInfoWidget> tempSessionInfoWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/Server/WB_SessionInfoWidget.WB_SessionInfoWidget_C'"));
	if (tempSessionInfoWidget.Succeeded()) {
		sessionInfoWidget = tempSessionInfoWidget.Class;
	}

}

void UStartLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayStartAnim();
	bt_Play->OnHovered.AddDynamic(this, &UStartLoginWidget::OnPlay);
	bt_Play->OnUnhovered.AddDynamic(this, &UStartLoginWidget::UnPlay);
	bt_Play->OnClicked.AddDynamic(this, &UStartLoginWidget::ClickedPlay);
	bt_Play->OnPressed.AddDynamic(this, &UStartLoginWidget::PressedPlay);
	bt_Play->OnReleased.AddDynamic(this, &UStartLoginWidget::ReleasedPlay);

	bt_nameNext->OnClicked.AddDynamic(this, &UStartLoginWidget::NameNext);
	
	btn_CreateSession->OnClicked.AddDynamic(this, &UStartLoginWidget::OnClickCreateButton);
	slider_playerCount->OnValueChanged.AddDynamic(this, &UStartLoginWidget::OnSliderMoved);
	btn_CreateSelection->OnClicked.AddDynamic(this, &UStartLoginWidget::OnClickedCreateSelection);
	btn_FindSelection->OnClicked.AddDynamic(this, &UStartLoginWidget::OnClickedFindSelection);
	btn_FindSession->OnClicked.AddDynamic(this, &UStartLoginWidget::OnClickFindButton);
	btn_BackFromCreate->OnClicked.AddDynamic(this, &UStartLoginWidget::BackToFirstCanvas);
	btn_BackFromFind->OnClicked.AddDynamic(this, &UStartLoginWidget::BackToFirstCanvas);

	gi = GetGameInstance<UNetGameInstance>();

	if (gi != nullptr)
	{
		gi->onSearchCompleted.AddDynamic(this, &UStartLoginWidget::AddRoomSlot);
		gi->onFindButtonActivation.AddDynamic(this, &UStartLoginWidget::ChangeButtonActivation);
	}

	bt_1Player->OnClicked.AddDynamic(this, &UStartLoginWidget::Selection1Player);
	bt_1Player->OnHovered.AddDynamic(this, &UStartLoginWidget::Selection1Player);
	bt_2Player->OnClicked.AddDynamic(this, &UStartLoginWidget::Selection2Player);
	bt_2Player->OnHovered.AddDynamic(this, &UStartLoginWidget::Selection2Player);
	bt_3Player->OnClicked.AddDynamic(this, &UStartLoginWidget::Selection3Player);
	bt_3Player->OnHovered.AddDynamic(this, &UStartLoginWidget::Selection3Player);

	UGameplayStatics::PlaySound2D(this,StartSound);

}

void UStartLoginWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
}


void UStartLoginWidget::OnClickCreateButton()
{
	SwitchCanvas(5);
	PlayAnimationForward(BeginAnim);
	/*UGameplayStatics::PlaySound2D(this, SelectionSound);*/
	/*if (gi != nullptr && !edit_roomName->GetText().IsEmpty())
	{
		gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
	}*/
}

void UStartLoginWidget::OnSliderMoved(float value)
{
	text_sliderCount->SetText(FText::AsNumber((int32)value));
}


void UStartLoginWidget::OnClickedCreateSelection()
{
	SwitchCanvas(3); //
}

void UStartLoginWidget::OnClickedFindSelection()
{
	SwitchCanvas(4);//
	OnClickFindButton();
}

void UStartLoginWidget::OnClickFindButton()
{
	if (gi != nullptr)
	{
		gi->FindOtherSession();
		sb_RoomListBox->ClearChildren();
	}
}

void UStartLoginWidget::BackToFirstCanvas()
{
	ws_SwitchCanvas->SetActiveWidgetIndex(1);
}

void UStartLoginWidget::AddRoomSlot(struct FSessionSlotInfo slotInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("AddRoomSlot!!"))
	// 슬롯 위젯을 생성한다.
	sessionSlot = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidget);

	if (sessionSlot != nullptr)
	{
		// 슬롯 위젯의 각 요소에 슬롯 정보를 넣는다.
		UE_LOG(LogTemp, Warning, TEXT("slotRoomName: %s"), *slotInfo.roomName)
		sessionSlot->text_RoomName->SetText(FText::FromString(slotInfo.roomName));
		sessionSlot->text_HostName->SetText(FText::FromString(slotInfo.hostName));
		sessionSlot->text_PlayerCount->SetText(FText::FromString(slotInfo.playerCount));
		sessionSlot->text_PingSpeed->SetText(FText::AsNumber(slotInfo.pingSpeed));
		sessionSlot->sessionIndex = slotInfo.sessionIndex;
		
		// 생성한 슬롯 위젯을 스크롤 박스에 자식으로 추가한다.
		sb_RoomListBox->AddChild(sessionSlot);
	}
}

void UStartLoginWidget::ChangeButtonActivation(bool bIsActivation)
{
	btn_FindSession->SetIsEnabled(bIsActivation);
}

void UStartLoginWidget::OnPlay()
{
	PlayAnimationForward(playAnim);

}

void UStartLoginWidget::UnPlay()
{
	PlayAnimationReverse(playAnim);
}


void UStartLoginWidget::ClickedPlay()
{
  // 플레이 클릭
  SwitchCanvas(1);
}

void UStartLoginWidget::PressedPlay()
{
	PlayAnimationForward(clickPlayAnim);
}

void UStartLoginWidget::ReleasedPlay()
{
	PlayAnimationReverse(clickPlayAnim);
}

void UStartLoginWidget::NameNext()
{ 
	if (!edit_userName->GetText().IsEmpty())
	{
		if (gi) {
			gi->mySessionName = edit_userName->GetText().ToString();
			SwitchCanvas(2);

		}
	}
}

void UStartLoginWidget::PlayStartAnim()
{
	PlayAnimationForward(StartAnim);
	FTimerHandle DelayHandle;
	
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				img_Ground->SetVisibility(ESlateVisibility::Hidden);
			}), 1, false);

}

void UStartLoginWidget::SwitchCanvas(int32 index)
{
	ws_SwitchCanvas->SetActiveWidgetIndex(index);
}


void UStartLoginWidget::Selection1Player()
{
	PlayAnimationForward(SelectionPlayer1Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UStartLoginWidget::Selection2Player()
{
	PlayAnimationForward(SelectionPlayer2Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UStartLoginWidget::Selection3Player()
{
	PlayAnimationForward(SelectionPlayer3Anim);
	bt_SelectionLok->SetVisibility(ESlateVisibility::Hidden);
}

void UStartLoginWidget::EnterInGame()
{
	if (!bStartChk) { // 한번만 로그인 클릭할 수 있게 처리
		bStartChk = true;
		LoadingIcon->SetRenderOpacity(1);
		if (gi) {
			if (!edit_roomName->GetText().IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Edit Room Name : %s / PlayerCOunt: %d"), *edit_roomName->GetText().ToString(), (int32)slider_playerCount->GetValue())
				gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
			}
		}
	}
}
