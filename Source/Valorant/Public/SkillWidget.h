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

	//UFUNCTION(BlueprintCallable)
	//void SetHpText();

	UPROPERTY(VisibleAnywhere, Category = MySettings)
	class ABaseCharacter* me;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_CGage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_QGage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_EGage;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_CGage_green;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_QGage_green;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_EGage_green;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
    class UImage* img_bottom_line;
	/// <summary>
	/// 
	/// </summary>
	/*UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
		class UImage* img_bullet;*/
//SB
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
};
