// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinLoseWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UWinLoseWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy)
	class UOverlay * OV_standBy;
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy)
	class UOverlay * OV_Win;
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy)
	class UOverlay * OV_Loss;

	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy) 
	class UOverlay * OV_WinBack;
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy)
	class UOverlay * OV_LoseBack;

	//Ending
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy) 
	class UOverlay * OV_LoseEnding;
	UPROPERTY(EditAnywhere, meta = (BindWidget),Category = StandBy)
	class UOverlay * OV_WinEnding;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* WinAnimation;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* LossAnimation;

	//Ending
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* WinEndingAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* LoseEndingAnim;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* WinBackGroundAnim;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,Category = StandBy)
	class UWidgetAnimation* LoseBackGroundAnim;

	UPROPERTY(VisibleAnywhere, Category = MySettings)
	class ABaseCharacter* me;
	UPROPERTY()
		class APlayerController* MyPlayerController;

	float curTime = 0;
public:
	UFUNCTION()
	void StartWinAnim();

	UFUNCTION()
	void StartLoseAnim();

	UFUNCTION()
	void onDestory();

	FWidgetAnimationDynamicEvent AnimFinshed;

	UFUNCTION()
	void StartLoseBackAnim();

	UFUNCTION()
	void StartWinBackAnim();

	UPROPERTY()
	class USoundBase* soundV;

	//Ending
	UFUNCTION()
	void StartWinEndingAnim();
	UFUNCTION()
	void StartLoseEndingAnim();

public:
	UPROPERTY()
	class UUMGSequencePlayer* SequencePlayer;

	void OnSequenceFinished(class UUMGSequencePlayer& InSequencePlayer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUMGSequencePlayer* WinPlayer;

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* WinnerNick;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* LoserNick;

	FTimerHandle SetWinLoseNickDelay;
	FString MyNick;
};
