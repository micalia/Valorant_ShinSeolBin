// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_BlueprintFunctionLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "EditorSupportDelegates.h"
#include "Editor.h"

void USB_BlueprintFunctionLibrary::SetStaticSwitchParameterValueInEditor(UMaterialInstance* MaterialInstance, const FName ParameterName, 	const bool bValue)
{
	if (MaterialInstance == nullptr)
	{
		return;
	}
	FStaticParameterSet StaticParameters = MaterialInstance->GetStaticParameters();

	bool bHasOverrideValue = false;
	for (auto& SwitchParameter : StaticParameters.StaticSwitchParameters)
	{
		if (SwitchParameter.ParameterInfo.Name.IsEqual(ParameterName))
		{
			SwitchParameter.Value = bValue;
			bHasOverrideValue = true;
			break;
		}
	}
	if (bHasOverrideValue == false)
	{
		FStaticSwitchParameter NewParam;
		NewParam.ParameterInfo.Name = ParameterName;
		NewParam.Value = bValue;
		NewParam.bOverride = true;
		NewParam.ParameterInfo.Index = INDEX_NONE;
		StaticParameters.StaticSwitchParameters.Add(NewParam);
	}
	MaterialInstance->MarkPackageDirty();
	MaterialInstance->PreEditChange(nullptr);
	MaterialInstance->PostEditChange();
	MaterialInstance->UpdateStaticPermutation(StaticParameters);
	
	FEditorDelegates::RefreshEditor.Broadcast();
	FEditorSupportDelegates::RedrawAllViewports.Broadcast();
}
