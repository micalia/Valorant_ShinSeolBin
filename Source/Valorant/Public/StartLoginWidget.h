// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CircularThrobber.h"
#include "StartLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UStartLoginWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UStartLoginWidget(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere,meta = (BindeWidget), Category = widget)
	class UImage* startimge;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim), Transient,Category = widgetAnim)
	class  UWidgetAnimation* StartAnim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim), Transient, Category = widgetAnim)
	class  UWidgetAnimation* playAnim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim), Transient, Category = widgetAnim)
	class  UWidgetAnimation* clickPlayAnim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim), Transient, Category = widgetAnim)
	class  UWidgetAnimation* BeginAnim;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
	class UImage* sova;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
	class UImage* neon;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
	class UImage* omen;

	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
		class UTextBlock* text_Play;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
		class UButton* bt_Play;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
	class UImage* img_Ground;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = widgetButton)
		class UButton* bt_nameNext;
		//새로추가 SB
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_CreateSession;
	/*UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class USlider* slider_playerCount;*/
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_CreateSelection;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FindSelection;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FindSession;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_BackFromCreate;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_BackFromFind;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UEditableText* edit_userName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UEditableText* edit_roomName;
	/*UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_sliderCount;*/

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UScrollBox* sb_RoomListBox;

public:
//캐릭터 선택창
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = button)
	class UButton* bt_1Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_2Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_3Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_Selection;
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_SelectionLok;

	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer1Anim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer2Anim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer3Anim;
	
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UCircularThrobber* LoadingIcon;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundWave* StartSound;
	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundWave* SelectionSound;

	class UAudioComponent* PlayedAudioComponent;

	UFUNCTION()
	void Selection1Player();
	UFUNCTION()
	void Selection2Player();
	UFUNCTION()
	void Selection3Player();
	
	bool bStartChk;
	UFUNCTION(BlueprintCallable)
	void EnterInGame();

public:
UFUNCTION()
	void OnClickCreateButton();

	UFUNCTION()
	void OnSliderMoved(float value);

	UFUNCTION()
	void OnClickedCreateSelection();

	UFUNCTION()
	void OnClickedFindSelection();

	UFUNCTION()
	void OnClickFindButton();

	UFUNCTION()
	void BackToFirstCanvas();

	UFUNCTION()
	void AddRoomSlot(struct FSessionSlotInfo slotInfo);

	UFUNCTION()
	void ChangeButtonActivation(bool bIsActivation);
	UPROPERTY(EditAnywhere, Category=MySettings)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidget;
	UPROPERTY()
	class USessionInfoWidget* sessionSlot;
public:
	UPROPERTY()
	class UNetGameInstance* gi;

	// 플레이 버튼 관련
	UFUNCTION()
	void OnPlay();
	UFUNCTION()
	void UnPlay();
	UFUNCTION()
	void ClickedPlay();
	UFUNCTION()
	void PressedPlay();
	UFUNCTION()
	void ReleasedPlay();

	UFUNCTION()
	void NameNext();
private:
	UFUNCTION()
	void PlayStartAnim();

public:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* ws_SwitchCanvas;
UFUNCTION(BlueprintCallable)
void SwitchCanvas(int32 index);
};
