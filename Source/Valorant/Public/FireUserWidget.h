// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FireUserWidget.generated.h"

class USizeBox;
class APawn;

UCLASS()
class VALORANT_API UFireUserWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_Ammo = 0;

public:
	UFireUserWidget(const FObjectInitializer& objectInitializer);
	UPROPERTY(EditAnywhere, Category = crosshair)
		float TestVelocity;

	UPROPERTY(EditAnywhere,Category = crosshair)
	bool isFire = false;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	USizeBox* SB_Top;
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	USizeBox* SB_Right;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
 	USizeBox * SB_Left;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	USizeBox * SB_Bottom;

	UPROPERTY(EditAnywhere,Category = crosshair)
	float MaxVelocity;
	UPROPERTY(EditAnywhere,Category = crosshair)
	float MaxOffset;
	UPROPERTY(EditAnywhere,Category = crosshair)
	float UpdatePerSecond;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
	APawn* owningPawn;

	FTimerHandle TcrossHair;

	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void HandleCrosshairScale();
public:
	void HandleCrosshairScale(float velocity);

	UPROPERTY(VisibleAnywhere, Category = MySettings)
		class ABaseCharacter* player;
	UPROPERTY()
	class UPlayerFireComponent* fireComp;

private:
};
