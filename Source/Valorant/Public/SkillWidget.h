// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 *
 */
UCLASS()
class VALORANT_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UInGameTopUi* WB_TopUI;

	UPROPERTY(VisibleAnywhere, Category = MySettings)
	class ABaseCharacter* me;

	UPROPERTY()
	class APlayerController* MyPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UTextBlock* Hp_txt;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* AmmoCnt_txt;

	UPROPERTY()
	class UPlayerFireComponent* FireComp;

	UFUNCTION()
	void SetAmmoCount(int32 AmmoCnt);
};
