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
	class ABaseCharacter* UiOwner;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = MySettings)
	class UFireUserWidget* WBP_FireAim;

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

public: // 공격한 플레이어가 있는 방향 표시
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* DamageIndicator;

	UPROPERTY(EditAnywhere)
	float DamageUiActiveTime = 3;
	float DamageUiActiveCurrTime = 0;

	UPROPERTY()
	class ABaseCharacter* AttackPlayer;

	bool bActiveDamagedUI;

public: // 조준 사격
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* AimingPanel;

	void AimingPanelOn();
	void AimingPanelOff();

public:
	UPROPERTY()
	class UMaterialInstance* SuperSkillProgressBarMat;

	UPROPERTY()
	class UMaterialInstanceDynamic* ProgressBarDynamicMat;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillProgressBarImg;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PercentVal;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* SkillGaugeBar;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* DragonArrowUI;

	FLinearColor  CoolTimeColor = FLinearColor::FromSRGBColor(FColor(0x5D, 0x5D, 0x5D, 0xFF));
	FLinearColor  SkillAvailableColor = FLinearColor::FromSRGBColor(FColor(0x5F, 0xEE, 0xB8, 0xFF));

	UPROPERTY(meta = (BindWidget))
	class UImage* CSkill_Cool_Img;
	UPROPERTY(meta = (BindWidget))
	class UImage* ESkill_Cool_Img;
	UPROPERTY(meta = (BindWidget))
	class UImage* QSkill_Cool_Img;
	UPROPERTY(meta = (BindWidget))
	class UImage* FSkill_Cool_Img;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* C_CoolTime;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* E_CoolTime;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Q_CoolTime;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* F_CoolTime;
};
