// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_AirSmokeMarker.h"
#include "../Public/SB_Sova.h"

void USB_AirSmokeMarker::NativeConstruct()
{
	Super::NativeConstruct();
	if (auto PC = GetWorld()->GetFirstPlayerController()) {
		if (auto Player = Cast<ASB_Sova>(PC->GetPawn())) {
			Player->OnRemoveSmokerUI.BindUObject(this, &USB_AirSmokeMarker::RemoveThisUI);
		}
	}
}

void USB_AirSmokeMarker::RemoveThisUI()
{
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> RemoveThisUI"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	RemoveFromParent();
}
