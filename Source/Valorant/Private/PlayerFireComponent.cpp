// Fill out your copyright notice in the Description page of Project Settings.

/*
#include "PlayerFireComponent.h"
#include "BaseCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FireUserWidget.h"
#include <TimerManager.h>
#include <Camera/CameraComponent.h>
#include <Engine/DecalActor.h>
#include <Camera/CameraShakeBase.h>
#include <UMG/Public/Components/SizeBox.h>
#include <Engine/SkeletalMeshSocket.h>
#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include <DrawDebugHelpers.h>
#include <Components/ActorComponent.h>
#include "Components/SceneComponent.h"
#include <GameFramework/PlayerController.h>
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "GameFramework/Character.h"
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/PlayerState.h>
#include "SB_Sova.h"

UPlayerFireComponent::UPlayerFireComponent()
{
	static ConstructorHelpers::FClassFinder<UFireUserWidget> tempFireWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WBP_Fire.WBP_Fire_C'"));
	if (tempFireWidget.Succeeded()) {
		fireWidget = tempFireWidget.Class;
	}

	bWantsInitializeComponent = true;

	SetIsReplicatedByDefault(true);
}

void UPlayerFireComponent::InitializeComponent()
{
	Super::InitializeComponent();

	
}

void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<APlayerController>(me->GetController());
	
	if (me->HasAuthority())
	{
		me->CurrHP = me->FullHP;
	}

}

void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (owningWeapon == nullptr)return;
	if (me == nullptr)return;
	//if(me->TheEndGame)
	if (isFire)
	{
		Fire();
		
	}

}

//reload?
void UPlayerFireComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(me->Key_X_action, ETriggerEvent::Started, this, &UPlayerFireComponent::Reload);
	}
}

//XŰ        reload
void UPlayerFireComponent::Reload()
{
	if (me->HasAuthority()) {
		Server_Reload_Implementation();
	}
	else {
		Server_Reload();
	}
}

void UPlayerFireComponent::Server_Reload_Implementation()
{
	ammo = 30;
}
void UPlayerFireComponent::Fire()
{

	if (owningWeapon == nullptr)
		return;
	//RPC             
	if (me->HasAuthority())
	{
		ServerFire_Implementation();
	}
	else
	{
		ServerFire();
	}
}
void UPlayerFireComponent::ServerFire_Implementation()
{
	if (me->GetWorldTimerManager().IsTimerActive(fireDelay))
	{
		return;
	}
	if (ammo > 0)
	{
	if (me) {
		me->PlaySound();
	}
		ammo--;
		FVector startLoc = me->FollowCamera->GetComponentLocation();
		FVector endLoc = startLoc + me->FollowCamera->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		
		params.AddIgnoredActor(me);

		//---------effect-----------
		const USkeletalMeshSocket* FireSocket = owningWeapon->meshComp->GetSocketByName("Fire Location");

		FTransform SocketTransform = FireSocket->GetSocketTransform(owningWeapon->meshComp);
		FVector StartTl = SocketTransform.GetLocation();
		if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, params))
		{
			FString hitActor = hitInfo.GetActor()->GetName();
			if (ABaseCharacter* hitPawn = Cast<ABaseCharacter>(hitInfo.GetActor()))
			{
				hitPawn->ServerDamagedHealth(attackPower, me);
				ServerHitProcess();
			}

		}	
		ServerFireEffect(FireSocket, SocketTransform.GetLocation(), hitInfo.ImpactPoint, hitInfo.ImpactNormal.Rotation(), endLoc, hitInfo.bBlockingHit);
		//          
		if (me->HasAuthority()) {
			me->GetWorldTimerManager().ClearTimer(fireDelay);
			me->GetWorldTimerManager().SetTimer(fireDelay, FTimerDelegate::CreateLambda([&]() {
				bInDelay = !bInDelay;
				}), fireInterval, false);
		}
		MulticastFire();
	}
}

bool UPlayerFireComponent::ServerFire_Validate()
{
	return true;
}

void UPlayerFireComponent::MulticastFire_Implementation()
{
	// ִϸ  ̼ 
	if (fireMontage)
	{	
		me->PlayAnimMontage(fireMontage);
		me->PlayFpsMontage();
	}

}

void UPlayerFireComponent::ServerFireEffect_Implementation(const USkeletalMeshSocket* FireSocket, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (FireSocket)
	{

		MulticastFireEffect(p1,p2,p3,p4,bBlockingHit);
	}
}
void UPlayerFireComponent::MulticastFireEffect_Implementation(FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactParticles,
			p2,
			p3
		);
	}
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleFlash,
			p1
		);
	}
	//    
	UWorld* World = GetWorld();
	if (World)
	{
		FVector BeamEnd = p4;
		if (bBlockingHit)
		{
			BeamEnd = p2;
		}
		if (BeamParticles)
		{
			//  Ƽĳ  Ʈ
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				p1,
				FRotator::ZeroRotator,
				true
			);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

void UPlayerFireComponent::ServerHitProcess_Implementation()
{
	MulticastHitProcess();
}

void UPlayerFireComponent::MulticastHitProcess_Implementation()
{
	APlayerController* pc = me->GetController<APlayerController>();

	if (pc != nullptr && pc->IsLocalPlayerController())
	{
		pc->ClientStartCameraShake(FireShake);
	}
}

void UPlayerFireComponent::StopFire()
{
	if(me==nullptr) return;
	ASB_Sova* sova = Cast<ASB_Sova>(me);
	if (sova) {
		if (sova->fire_UI) {
			//  ݵ  ó          ư   
			if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController()) {

				isFire = false;

				//UI cross hair
				sova->fire_UI->isFire = false;
			}
		}

	}

	ASH_Neon* neon = Cast<ASH_Neon>(me);
	if (neon) {
		if (neon->fire_UI) {
			//  ݵ  ó          ư   
			if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController()) {
				isFire = false;

				//UI cross hair
				neon->fire_UI->isFire = false;
			}
		}
	}
	if (neon)
	{
		if (neon->fire_UI != nullptr) {
			//  ݵ  ó          ư   
			if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController())
				isFire = false;

			//UI cross hair
			neon->fire_UI->isFire = false;
		}
	}
	
	if (playerController)
	{
		//if (ammo > 0) {
		//	ī ޶   ö󰡰        
		//	//playerController->SetControlRotation(startFireRot);
		//}
		curRightRecoilAmount = 0;
		curRecoilAmount = 0;

		//isFireStart = false;
	}

}

void UPlayerFireComponent::PrintLog()
{
	const FString localRoleString = UEnum::GetValueAsString<ENetRole>(myLocalRole);
	const FString remoteRoleString = UEnum::GetValueAsString<ENetRole>(myRemoteRole);
	const FString ownerString = GetOwner() != nullptr ? GetOwner()->GetName() : FString("No Owner");
	const FString printString = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nOwner Name: %s\nNet Connection : %s"), *localRoleString, *remoteRoleString, *ownerString);
}

void UPlayerFireComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerFireComponent, owningWeapon);
	DOREPLIFETIME(UPlayerFireComponent, ammo);
	DOREPLIFETIME(UPlayerFireComponent, attackPower);
	DOREPLIFETIME(UPlayerFireComponent, fireInterval);
}
*/