// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SB_SovaAnim.generated.h"

UENUM(BlueprintType)
enum class ESovaWalkState :uint8
{
	IDLE = 0,
	WALKING,
	CROUCHING,
};
UCLASS()
class VALORANT_API USB_SovaAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ASB_Sova* me;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float direction = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MySettings)
	float deltaRot = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MySettings)
	bool bCrounch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MySettings)
	ESovaWalkState CurrWalkState = ESovaWalkState::IDLE;

	UFUNCTION()
	void AnimNotify_ReloadComplete();

	UFUNCTION()
	void AnimNotify_Die();

	UFUNCTION()
	void AnimNotify_FadeOut();

	UFUNCTION()
	void AnimNotify_PullArrow();
	UFUNCTION()
	void AnimNotify_ShotComplete();
	UFUNCTION()
	void AnimNotify_CanScoutingArrow();

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class USkillWidget> SkillWidgetFactory;
	//UPROPERTY()
	//class USkillWidget* skillWigetInstance;

};
