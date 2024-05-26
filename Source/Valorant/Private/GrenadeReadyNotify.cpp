// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeReadyNotify.h"
#include "../Public/SB_Sova.h"

void UGrenadeReadyNotify::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);
	
	if(ASB_Sova* Sova = Cast<ASB_Sova>(BranchingPointPayload.SkelMeshComponent->GetOwner())) {
		Sova->bThrowing = true;
		if(Sova->IsLocallyControlled()){
			Sova->ShowProjectilePath();
		}
	}
}
