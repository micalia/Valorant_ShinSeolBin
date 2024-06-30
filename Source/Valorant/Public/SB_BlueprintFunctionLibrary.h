// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SB_BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API USB_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
    static void SetStaticSwitchParameterValueInEditor(class UMaterialInstance* MaterialInstance, const FName ParameterName, const bool bValue);
};
