// Fill out your copyright notice in the Description page of Project Settings.


#include "Omen_Eskill.h"
#include "../Public/MH_Omen.h"
#include "BaseCharacter.h"
#include <GameFramework/PlayerController.h>
#include "Math/Vector.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "Camera/CameraComponent.h"
//#include "../Public/SB_Sova.h"
//#include "../Public/SH_Neon.h"
#include <Kismet/KismetMathLibrary.h>
#include <Delegates/Delegate.h>
#include "MHnSB_NSH.h"
#include "SH_Neon.h"
#include "Left_NSH.h"
//#include "Nasunhwan.h"

// Sets default values for this component's properties
UOmen_Eskill::UOmen_Eskill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UOmen_Eskill::InitializeComponent()
{
	me = Cast<ABaseCharacter>(GetOwner());
	me->onInputBindingDelegate.AddUObject(this, &UOmen_Eskill::SetupInputBinding);
}

// Called when the game starts
void UOmen_Eskill::BeginPlay()
{
	Super::BeginPlay();

	//omen=Cast<AMH_Omen>(me);
	neon=Cast<ASH_Neon>(me);
	
}

// Called every frame
void UOmen_Eskill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (me == nullptr)return;
	if (IsE)
	{
		CameraSwitching();
		if (distCameraToSphere() <= 200)
			IsBlocking = true;

		if (IsForward)
		{
			IsBlocking = false;
			CameraToMarbleDistance = Forwarding();
			neon->Sphere->SetWorldLocation(CameraPos());
		}
		if (IsBackward)
		{
			if (!IsBlocking)
			{
				CameraToMarbleDistance = Backwarding();
				neon->Sphere->SetWorldLocation(CameraPos());
			}
		}
	}
}

void UOmen_Eskill::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(me->mouseLeftAction, ETriggerEvent::Started, this, &UOmen_Eskill::LeftMouseAction);

		EnhancedInputComponent->BindAction(me->mouseLeftAction, ETriggerEvent::Completed, this, &UOmen_Eskill::LeftReleasedAction);

		EnhancedInputComponent->BindAction(me->mouseRightAction, ETriggerEvent::Started, this, &UOmen_Eskill::RightMouseAction);

		EnhancedInputComponent->BindAction(me->mouseRightAction, ETriggerEvent::Completed, this, &UOmen_Eskill::RightReleasedAction);

		EnhancedInputComponent->BindAction(me->Key_F_action, ETriggerEvent::Started, this, &UOmen_Eskill::KeyF);

		EnhancedInputComponent->BindAction(me->Key_R_action, ETriggerEvent::Started, this, &UOmen_Eskill::KeyR);

	}
}

void UOmen_Eskill::LeftMouseAction()
{
	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("LeftClick")), true, FVector2D(1, 1));*/
	IsForward = true;
/*	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("IsForward: %d"),IsForward), true, FVector2D(1, 1));*/
}

void UOmen_Eskill::LeftReleasedAction()
{
	IsForward = false;
}


void UOmen_Eskill::RightMouseAction()
{
	IsBackward=true;
}

void UOmen_Eskill::RightReleasedAction()
{
	IsBackward = false;
}

void UOmen_Eskill::KeyR()
{
	if (IsE)
	{
		SwitchMode(ESkill_Enum);
		if (ESkill_Enum == ESkill_EMode::ModeA)
		{
			CameraToCM();
		}else CameraToFC();
	}
}

void UOmen_Eskill::KeyF()
{
	if (!IsE)
	{
		IsE = true;
		if (neon)
		{
			neon->Sphere->SetVisibility(true);
			if (ESkill_Enum == ESkill_EMode::ModeB)
			{
				SwitchMode(ESkill_Enum);
			}
		}
		
	}
	else {
		if (SpawnedENSH == nullptr)
		{
			if (ESkill_Enum == ESkill_EMode::ModeA)
			{
				SwitchMode(ESkill_Enum);
				CameraToFC();
			}
			neon->Sphere->SetVisibility(false);

			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector dir = (neon->Sphere->GetComponentLocation() - neon->NSHLeftPos->GetComponentLocation());
			dir.Normalize(0.0001);

			FRotator rotator = UKismetMathLibrary::MakeRotFromX(dir);
			FVector spawnLocation = neon->NSHLeftPos->GetComponentLocation();

			SpawnedENSH = GetWorld()->SpawnActor<AMHnSB_NSH>(SpawnedNSHFactory, spawnLocation, rotator, spawnParams);

			if (SpawnedENSH)
			{
				SpawnedENSH->TargetPos = neon->Sphere->GetComponentLocation();
				/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("nsh!!!!")), true, FVector2D(1, 1));*/
				AActor* nsh = Cast<AActor>(SpawnedENSH);
				nsh->OnDestroyed.AddDynamic(this, &UOmen_Eskill::MyDestroyed);
			}
		}
	}
}

void UOmen_Eskill::CameraSwitching()
{
	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("CameraSwitching")),true, FVector2D(1, 1));*/
	if (ESkill_Enum == ESkill_EMode::ModeA && distCameraToSphere() >= 400 && neon->FollowCamera->IsActive())
	{
		CameraToCM();
	}
	else if (distCameraToSphere() < 400 && neon->CM->IsActive())
	{
		CameraToFC();
	}
}

void UOmen_Eskill::CameraToFC()
{
	neon->CM->Deactivate();
	neon->FollowCamera->SetActive(true);
}

void UOmen_Eskill::CameraToCM()
{
	neon->FollowCamera->Deactivate();
	neon->CM->SetActive(true);
}

FVector UOmen_Eskill::CameraPos()
{
	return (neon->FollowCamera->GetForwardVector()) * CameraToMarbleDistance + (neon->FollowCamera->GetComponentLocation());
}

float UOmen_Eskill::Forwarding()
{
	return CameraToMarbleDistance + ((GetWorld()->DeltaTimeSeconds) * Speed);
}

float UOmen_Eskill::Backwarding()
{
	return CameraToMarbleDistance + ((GetWorld()->DeltaTimeSeconds) * (Speed) * (-1));
}

float UOmen_Eskill::distCameraToSphere()
{
	float dist = FVector::Distance(neon->Sphere->GetComponentLocation(), me->FollowCamera->GetComponentLocation());

	return dist;
}

void UOmen_Eskill::SwitchMode(ESkill_EMode Mode)
{
	if (Mode == ESkill_EMode::ModeB)
	{
		ESkill_Enum = ESkill_EMode::ModeA;
		CameraModeLevel();
	}
	else {
		ESkill_Enum = ESkill_EMode::ModeB;
		PlayerModeLevel();
	}
}

void UOmen_Eskill::MyDestroyed(AActor* nshParam)
{
	IsE = false;
	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("destroyed!")), true, FVector2D(1, 1));*/
}