// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_SovaAnim.h"
#include "SB_Sova.h"
#include "PlayerFireComponent.h"

void USB_SovaAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	me = Cast<ASB_Sova>(TryGetPawnOwner());
}

void USB_SovaAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (me) {
		FVector forward = me->GetActorForwardVector();
		FVector velocity = me->GetVelocity();
		speed = FMath::Clamp(FVector::DotProduct(forward, velocity), -100, 100);
		FVector right = me->GetActorRightVector();
		direction = FMath::Clamp(FVector::DotProduct(right, velocity), -100, 100);
		FRotator delta = (me->GetActorRotation() - me->GetBaseAimRotation()).GetNormalized();
		deltaRot = delta.Pitch;
		
		bCrounch =  me->bSovaCrounch;
	}

}

void USB_SovaAnim::AnimNotify_ReloadComplete()
{
	if(!me->IsLocallyControlled())return;
	if (me->HasAuthority()) {
		me->fireComp->ServerReloadComplete_Implementation();
	}
	else {
		me->fireComp->ServerReloadComplete();
	}
	me->ChangeCurrState(ESovaState::DefaultAtk);
}

void USB_SovaAnim::AnimNotify_Die()
{
	//fade out animation 
	
	if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController())
	{
		me->EnableInput(me->GetController<APlayerController>());
	}
	
	if (!me->TheEndGame) {
		me->RestartProcess();
	}
}

void USB_SovaAnim::AnimNotify_FadeOut()
{
	if (me->IsLocallyControlled())
	{
		me->PlayDieAnimationInWidget();
	}
	
}

void USB_SovaAnim::AnimNotify_PullArrow()
{
	if (auto ArrowAnim = me->arrowMesh->GetAnimInstance()) {
		ArrowAnim->Montage_Play(me->OnlyArrowMontage);
	}
}

void USB_SovaAnim::AnimNotify_ShotComplete()
{
	me->isGun = true;
	me->arrowMesh->SetVisibility(false);
	me->BaseWeapon->SetVisibility(true);
	me->ChangeCurrState(ESovaState::DefaultAtk);
}

void USB_SovaAnim::AnimNotify_CanScoutingArrow()
{
	me->SetBoolScoutingArrow(true);
}