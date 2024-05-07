// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSH_SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UPSH_SkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UProgressBar* pb_EnegeBar;

	UPROPERTY(EditAnywhere)
	class ASH_Neon* player;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
		class APlayerController* MyPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget), Category = MySettings)
		class UTextBlock* Hp_txt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
		class UTextBlock* MyNameAndScore_txt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
		class UTextBlock* EnemyNameAndScore_txt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
		class UTextBlock* MyScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
		class UTextBlock* EnemyScore;
private:

};
