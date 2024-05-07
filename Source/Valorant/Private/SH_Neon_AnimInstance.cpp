// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Neon_AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SH_Neon.h"
#include <GameFramework/PlayerController.h>



USH_Neon_AnimInstance::USH_Neon_AnimInstance()
{

	//1P
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempF_skill_Q_Montage(TEXT("/Script/Engine.AnimMontage'/Game/PSH/anim/New1P/MT_New_1P_Skillq.MT_New_1P_Skillq'"));
	if (tempF_skill_Q_Montage.Succeeded())
	{
		F_skill_Q_Montage = tempF_skill_Q_Montage.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempF_Skill_C_Montage(TEXT("/Script/Engine.AnimMontage'/Game/PSH/anim/New1P/MT_New_1P_SkillC.MT_New_1P_SkillC'"));
	if (tempF_Skill_C_Montage.Succeeded())
	{
		F_Skill_C_Montage = tempF_Skill_C_Montage.Object;
	}

	//3P
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempT_Fire(TEXT("/Script/Engine.AnimMontage'/Game/PSH/anim/3P/MT_3P_Fire.MT_3P_Fire'"));
	if (tempT_Fire.Succeeded())
	{
		T_Fire = tempT_Fire.Object;
	}

}

void USH_Neon_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	player = Cast<ASH_Neon>(GetOwningActor());

	if (player != nullptr)
	{
		movementComp = player->GetCharacterMovement();
	}
}

void USH_Neon_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player != nullptr && movementComp != nullptr)
	{

		speed = movementComp->Velocity.Size2D();
		degree = CalculateDir(movementComp->Velocity, player->GetActorRotation());

		FRotator delta = (player->GetActorRotation() - player->GetBaseAimRotation()).GetNormalized();
		deltaRot = delta.Pitch;

	}
}


void USH_Neon_AnimInstance::Play_1P_Skill_Q_Animation()
{
	Montage_Play(F_skill_Q_Montage);
	player->BP_Play_1P_Skill_Q_Animation();
}

void USH_Neon_AnimInstance::Play_1P_Skill_C_Animation()
{
	Montage_Play(F_Skill_C_Montage);
	player->BP_Play_3P_Skill_C_Animation();
}


void USH_Neon_AnimInstance::Play_3P_Fire_Animation()
{
	Montage_Play(T_Fire);
}

void USH_Neon_AnimInstance::AnimNotify_W_Hide()
{
	player->HasWeapon = false;
}

void USH_Neon_AnimInstance::AnimNotify_W_visi()
{
	player->HasWeapon = true;
}	

void USH_Neon_AnimInstance::AnimNotify_Die()
{
	if (player->GetController() != nullptr && player->GetController()->IsLocalPlayerController())
	{
		player->EnableInput(player->GetController<APlayerController>());
	}

	player->RestartProcess();
}

void USH_Neon_AnimInstance::AnimNotify_FadeOut()
{
	if (player->IsLocallyControlled())
	{
		player->PlayDieAnimationInWidget();
	}
}

float USH_Neon_AnimInstance::CalculateDir(FVector velocity, FRotator rot)
{
	if (velocity.IsNearlyZero())
	{
		return 0.0f;
	}

	FVector forwardVector = FRotationMatrix(rot).GetUnitAxis(EAxis::X);
	FVector rightVector = FRotationMatrix(rot).GetUnitAxis(EAxis::Y);
	FVector speedVector = velocity.GetSafeNormal2D();

	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(forwardVector, speedVector)));

	float rightDot = FVector::DotProduct(rightVector, speedVector);
	if (rightDot < 0)
	{
		angle *= -1.0f;
	}

	return angle;
}
