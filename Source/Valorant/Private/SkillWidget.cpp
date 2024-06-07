// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "PlayerFireComponent.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include "SB_Sova.h"
#include "InGameTopUi.h"
#include "PlayerFireComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (MyPlayerController) {
		UiOwner = Cast<ABaseCharacter>(MyPlayerController->GetPawn());
		if (UiOwner) {
			FireComp = Cast<UPlayerFireComponent>(UiOwner->GetComponentByClass(UPlayerFireComponent::StaticClass()));
			if (FireComp)
			{
				FireComp->SetAmmoCountTextInit(this);
				GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Get FireComp"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Not FireComp"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
			}
		}
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass() ,OutActors);
	for (auto& Player: OutActors)
	{
		ABaseCharacter* ValorantPlayer = Cast<ABaseCharacter>(Player);
		if (!ValorantPlayer->IsLocallyControlled()) {
			AttackPlayer = ValorantPlayer;
		}
	}
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	

	if (AttackPlayer == nullptr) {
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), OutActors);
		for (auto& Player : OutActors)
		{
			ABaseCharacter* ValorantPlayer = Cast<ABaseCharacter>(Player);
			if (!ValorantPlayer->IsLocallyControlled()) {
				AttackPlayer = ValorantPlayer;
			}
		}
	}

	if (WB_TopUI && UiOwner) {
		auto GameStateVal = GetWorld()->GetGameState();
		if (GameStateVal) {
			TArray<APlayerState*> players = GameStateVal->PlayerArray;
			FString playersInfoText;

			for (APlayerState* p : players)
			{
				auto a = MyPlayerController->GetPawn();
				if (a) {
					if (p != MyPlayerController->GetPawn()->GetPlayerState()) {
						WB_TopUI->EnemyName_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *p->GetPlayerName())));
						WB_TopUI->EnemyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)p->GetScore())));
					}
				}
			}
		}

		// 플레이어의 총알 정보 갱신
		if (UiOwner->GetPlayerState()) {
			WB_TopUI->MyName_txt->SetText(FText::FromString(FString::Printf(TEXT("%s"), *UiOwner->GetPlayerState()->GetPlayerName())));
			WB_TopUI->MyScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)UiOwner->GetPlayerState()->GetScore())));
		}

		if (Hp_txt) {
			Hp_txt->SetText(FText::AsNumber(UiOwner->GetHP()));
		}
	}

#pragma region AttackIndicator
	if (bActiveDamagedUI) {
		DamageIndicator->SetRenderOpacity(1);
		if (AttackPlayer) {
			DamageUiActiveCurrTime += InDeltaTime;
			if(DamageUiActiveCurrTime > DamageUiActiveTime) {
				DamageUiActiveCurrTime = 0;
				bActiveDamagedUI = false;
				DamageIndicator->SetRenderOpacity(0);
			}
			FVector2D MyForwardVec2D = FVector2D(UiOwner->GetActorForwardVector().X, UiOwner->GetActorForwardVector().Y);
			FVector ToAttackPlayerDir = AttackPlayer->GetActorLocation() - UiOwner->GetActorLocation();
			ToAttackPlayerDir.Normalize();
			FVector2D ToAttackPlayerDir2D = FVector2D(ToAttackPlayerDir.X, ToAttackPlayerDir.Y);
			float Cross = UKismetMathLibrary::CrossProduct2D(MyForwardVec2D, ToAttackPlayerDir2D);
			float Dot = UKismetMathLibrary::DotProduct2D(MyForwardVec2D, ToAttackPlayerDir2D);
			float RotateAngle = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::Acos(Dot));
			
			if (Cross < 0) {
				RotateAngle *= -1;
				DamageIndicator->SetRenderTransformAngle(RotateAngle);
			}
			else {
				DamageIndicator->SetRenderTransformAngle(RotateAngle);
			}
		}
	}
#pragma endregion
}

void USkillWidget::SetAmmoCount(int32 AmmoCnt)
{
	AmmoCnt_txt->SetText(FText::AsNumber(AmmoCnt));
}

void USkillWidget::AimingPanelOn()
{
	AimingPanel->SetVisibility(ESlateVisibility::Visible);
	if (APlayerCameraManager* PlayerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)) {
		PlayerCam->SetFOV(45);
	}
}

void USkillWidget::AimingPanelOff()
{
	AimingPanel->SetVisibility(ESlateVisibility::Hidden);
	if (APlayerCameraManager* PlayerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)) {
		PlayerCam->SetFOV(0);
	}
}
