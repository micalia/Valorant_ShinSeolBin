// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RespawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API URespawnWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_Retry;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
private:
	class ABaseCharacter* player;

	//UFUNCTION()
	//void OnExitSession();

	UFUNCTION()
	void OnClickedRetry();
};
