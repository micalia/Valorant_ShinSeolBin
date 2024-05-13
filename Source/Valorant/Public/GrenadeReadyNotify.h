// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "GrenadeReadyNotify.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UGrenadeReadyNotify : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()
	
public:
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
