// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeReadyNotify.h"
#include "../Public/SB_Sova.h"

void UGrenadeReadyNotify::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Greande Notify"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	
	if(ASB_Sova* Sova = Cast<ASB_Sova>(BranchingPointPayload.SkelMeshComponent->GetOwner())) {
		Sova->bThrowing = true;
		if(Sova->IsLocallyControlled()){
			Sova->DebugSphere();
			Sova->ShowProjectilePath();
			GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Show Projectile Path!!"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
		}
	}
}
