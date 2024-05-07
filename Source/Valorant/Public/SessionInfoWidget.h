// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API USessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_RoomName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_HostName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_PlayerCount;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UTextBlock* text_PingSpeed;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Join;

	int32 sessionIndex = 0;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UNetGameInstance* gi;

	UFUNCTION()
	void OnClickedJoinButton();
};

