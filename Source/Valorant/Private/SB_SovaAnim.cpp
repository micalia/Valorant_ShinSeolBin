// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_SovaAnim.h"
#include "SB_Sova.h"
#include "NetPlayerController.h"
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
		speed = FMath::Clamp(FVector::DotProduct(forward, velocity), -280, 280);

		FVector right = me->GetActorRightVector();
		direction = FMath::Clamp(FVector::DotProduct(right, velocity), -280, 280);

		FRotator delta = (me->GetActorRotation() - me->GetBaseAimRotation()).GetNormalized();
		deltaRot = delta.Pitch;
	}
}

void USB_SovaAnim::AnimNotify_ReloadComplete()
{
	if (me->HasAuthority()) {
		me->fireComp->ServerReloadComplete_Implementation();
	}
	else {
		me->fireComp->ServerReloadComplete();
	}
}

void USB_SovaAnim::AnimNotify_Die()
{
	/*//GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, TEXT("nofify !!!"), true, FVector2D(1, 1));*/

	//fade out animation 
	
	if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController())
	{
		me->EnableInput(me->GetController<APlayerController>());
	}
	
	me->RestartProcess();
}

void USB_SovaAnim::AnimNotify_FadeOut()
{
	if (me->IsLocallyControlled())
	{
		me->PlayDieAnimationInWidget();
	}
	
}

