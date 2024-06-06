// Fill out your copyright notice in the Description page of Project Settings.


#include "FireUserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"
#include "PlayerFireComponent.h"

UFireUserWidget::UFireUserWidget(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
	/*TestVelocity = 0.0f;
	MaxVelocity = 350.0f;
	MaxOffset = 100.0f;

	UpdatePerSecond = 60.0f;*/
}

void UFireUserWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	//HandleCrosshairScale(TestVelocity);
}

void UFireUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
		if (APawn* Pawn = PC->GetPawn()) {
			player = Cast<ABaseCharacter>(Pawn);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TcrossHair, this, &UFireUserWidget::HandleCrosshairScale, 1.0f/UpdatePerSecond, true);*/
}

void UFireUserWidget::HandleCrosshairScale()
{
	if (player)
	{
		if (!isFire)
		{
			TestVelocity = player->GetVelocity().Size();
		}
		else
		{
			TestVelocity = FMath::RandRange(75.0f, 125.0f);
		}
	}
	//HandleCrosshairScale(TestVelocity);
}
void UFireUserWidget::HandleCrosshairScale(float velocity)
{
	/*if (SB_Top && SB_Bottom && SB_Left && SB_Right)
	{
		float FixedVelocity = UKismetMathLibrary::NormalizeToRange(velocity,0.0f,MaxVelocity);
		FixedVelocity = FMath::Clamp(FixedVelocity,0.0f,1.0f);
		FixedVelocity *= MaxOffset;

		if (UCanvasPanelSlot* CPS_Top = Cast<UCanvasPanelSlot>(SB_Top->Slot))
		{
			FVector2D Position;
			Position.Y = -FixedVelocity;
			Position.X = 0.0f;
			CPS_Top->SetPosition(Position);
		}

		if (UCanvasPanelSlot* CPS_Bottom = Cast<UCanvasPanelSlot>(SB_Bottom->Slot))
		{
			FVector2D Position;
			Position.Y = FixedVelocity;
			Position.X = 0.0f;
			CPS_Bottom->SetPosition(Position);
		}

		if (UCanvasPanelSlot* CPS_Left = Cast<UCanvasPanelSlot>(SB_Left->Slot))
		{
			FVector2D Position;
			Position.Y =  0.0f;
			Position.X = -FixedVelocity;
			CPS_Left->SetPosition(Position);
		}

		if (UCanvasPanelSlot* CPS_Right = Cast<UCanvasPanelSlot>(SB_Right->Slot))
		{
			FVector2D Position;
			Position.Y = 0.0;
			Position.X = FixedVelocity;
			CPS_Right->SetPosition(Position);
		}
	}*/

}

void UFireUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 플레이어의 총알 정보 갱신
	if (player != nullptr)
	{
		fireComp = Cast<UPlayerFireComponent>(player->GetComponentByClass(UPlayerFireComponent::StaticClass()));
		if (fireComp) {
			//			text_Ammo->SetText(FText::AsNumber(fireComp->GetAmmo()));
		}
	}
	else {
		player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

}
