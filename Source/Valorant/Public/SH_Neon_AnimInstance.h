// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SH_Neon_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API USH_Neon_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	USH_Neon_AnimInstance();
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ASH_Neon * player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Settings")
	float speed = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Settings")
	float degree= 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bHasPistol = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float deltaRot = 0;

	UPROPERTY(EditAnywhere,Category="1PMontage")
	class UAnimMontage* F_skill_Q_Montage;
	UPROPERTY(EditAnywhere,Category="1PMontage")
	class UAnimMontage* F_Skill_C_Montage;
	UPROPERTY(EditAnywhere,Category="3PMontage")
	class UAnimMontage* T_Fire;


	UPROPERTY()
	class UCharacterMovementComponent* movementComp;


	float CalculateDir(FVector velocity, FRotator rot);


	void Play_1P_Skill_Q_Animation();

	void Play_1P_Skill_C_Animation();

	void Play_3P_Fire_Animation();

// AnimNotify
	
	UFUNCTION()
	void AnimNotify_W_Hide();

	UFUNCTION()
	void AnimNotify_W_visi();
	
	UFUNCTION()
	void AnimNotify_Die();

	UFUNCTION()
	void AnimNotify_FadeOut();

};
