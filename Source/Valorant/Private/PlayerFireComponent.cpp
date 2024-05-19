// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "BaseCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FireUserWidget.h"
#include <TimerManager.h>
#include <Camera/CameraComponent.h>
#include <Engine/DecalActor.h>
#include <Camera/CameraShakeBase.h>
#include "PSH_Weapon.h"
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
#include "SH_Neon_AnimInstance.h"
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/PlayerState.h>
#include "SB_Sova.h"
#include "SH_Neon.h"



UPlayerFireComponent::UPlayerFireComponent()
{
	static ConstructorHelpers::FClassFinder<UFireUserWidget> tempFireWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WBP_Fire.WBP_Fire_C'"));
	if (tempFireWidget.Succeeded()) {
		fireWidget = tempFireWidget.Class;
	}
	/*static ConstructorHelpers::FClassFinder<UCameraShakeBase> tempFireShake(TEXT("/Script/Engine.Blueprint'/Game/PSH/BluePrints/BP_HitShake.BP_HitShake_C'"));
	if (tempFireShake.Succeeded()) {
		FireShake = tempFireShake.Class;
	}*/

	bWantsInitializeComponent = true;
	//SetIsReplicated(true);

	SetIsReplicatedByDefault(true);
}

void UPlayerFireComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//AttachWeapon(_weapon);
	
}

void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<APlayerController>(me->GetController());

		//	if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController())
		//	{
		//		fire_UI = CreateWidget<UFireUserWidget>(GetWorld(), fireWidget);
		//		if (fire_UI != nullptr)
		//		{
		//			/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("widget ")), true, FVector2D(1, 1));*/
		//			fire_UI->AddToViewport();
		//		}
		//	}
		//	}), 1.5f, false);
	
	if (me->HasAuthority())
	{
		me->CurrHP = me->FullHP;
		/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("widget ")), true, FVector2D(1, 1));*/
	}

}

void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("me->TheEndGame: "),  /* value */), true, FVector2D(1, 1));
	//PrintLog();
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
		/*EnhancedInputComponent->BindAction(me->mouseLeftAction, ETriggerEvent::Started, this, &UPlayerFireComponent::BaseFire);*/
		/*EnhancedInputComponent->BindAction(me->mouseLeftAction, ETriggerEvent::Completed, this, &UPlayerFireComponent::StopFire);*/
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

//     ϴ     
//void UPlayerFireComponent::BaseFire()
//{
//	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("BaseFire!!!"), true, FVector2D(1, 1)));
//	UE_LOG(LogTemp, Warning, TEXT("BaseFire!!!"))
//	isFire = true;
//}

void UPlayerFireComponent::Fire()
{

	if (owningWeapon == nullptr)
		return;
	
		//USoundCue
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, me->GetActorLocation());
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

//
//void UPlayerFireComponent::WeaponInfoReset()
//{
//	ammo = 0;
//	fireInterval = 0;
//	attackPower = 0;
//}
//
//void UPlayerFireComponent::AttachWeapon(TSubclassOf<class ABaseWeapon> weapon)
//{
//	if (weapon)
//	{
//		// SpawnActor        weapon      ͸                      
//		/*_equipWeapon->SetOwner(me);
//		_equipWeapon->playerInstance = me;*/
//		//       ̸               ޽ÿ               
//		//if (me) {
//		//	const USkeletalMeshSocket* weaponSocket = me->GetMesh()->GetSocketByName("WeaponLoc");
//
//		//	if (_equipWeapon && weaponSocket)
//		//	{
//		//		//    Ͽ     ͸   Ҵ 
//		//		weaponSocket->AttachActor(_equipWeapon, me->GetMesh());
//		//	}
//		//}
//	}
//}

// ϴ          ,,
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

		//ī ޶󿡼   Ѿ   ߻ 
		FVector startLoc = me->FollowCamera->GetComponentLocation();
		FVector endLoc = startLoc + me->FollowCamera->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		
		params.AddIgnoredActor(me);

		// ѱ       Ѿ      (effect)
		//owningWeapon->ServerFireEffect(hitInfo, startLoc, endLoc);

		//---------effect-----------
		const USkeletalMeshSocket* FireSocket = owningWeapon->meshComp->GetSocketByName("Fire Location");

		FTransform SocketTransform = FireSocket->GetSocketTransform(owningWeapon->meshComp);
		FVector StartTl = SocketTransform.GetLocation();



		//        
		if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, params))
		{
			FString hitActor = hitInfo.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *hitActor);
			if (ABaseCharacter* hitPawn = Cast<ABaseCharacter>(hitInfo.GetActor()))
			{
				//        ó  
				hitPawn->ServerDamagedHealth(attackPower, me);
				////GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, FString::Printf(TEXT(" MyActor: %s / CurrHp: %f // HitActor: %s / EnemyCurrHp: %f"), *me->GetName(), me->CurrHP, *hitInfo.GetActor()->GetName(), hitPawn->CurrHP), true, FVector2D(1, 1));
				ServerHitProcess();
				//me->GetPlayerState();
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
	//else
	//{
	//	return; //   ź   0    ִ ?
	//}
	
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
		//anim->PlayFireAnimation(); // ERROR
		me->PlayFpsMontage();
		//anim->Montage_Play(fireMontage);
		//UE_LOG(LogTemp,Warning,TEXT("Fucking"));
	}

	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, TEXT("montage!!!!!!!!!!!!!"), true, FVector2D(1, 1));*/

}

void UPlayerFireComponent::ServerFireEffect_Implementation(const USkeletalMeshSocket* FireSocket, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (FireSocket)
	{
		/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, TEXT("11111111111111111111"), true, FVector2D(1, 1));*/

		MulticastFireEffect(p1,p2,p3,p4,bBlockingHit);
		//if (FireSound)
		//{
		//	/*UGameplayStatics::PlaySoundAtLocation(
		//		GetWorld(),
		//		FireSound,
		//		GetActorLocation()
		//	);*/
		//	//UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
		//	PlaySound();
		//	UE_LOG(LogTemp, Warning, TEXT("sound"));
		//}
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
		//battle_UI->PlayHitAnim();
		pc->ClientStartCameraShake(FireShake);
	}

	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("MULTICAStHitProcess")), true, FVector2D(1, 1));*/
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

//void UPlayerFireComponent::OnHitWall(const FHitResult& hitResult)
//{
//	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/PSH/Resouce/Bullethole/MT_BulletHole.MT_BulletHole'"));
//
//	if (DecalMaterial)
//	{
//		//   Į          
//		FTransform DecalTransform = FTransform(hitResult.ImpactNormal.Rotation(), hitResult.ImpactPoint);
//		ADecalActor* DecalActor = GetWorld()->SpawnActor<ADecalActor>(DecalTransform.GetLocation(), DecalTransform.GetRotation().Rotator());
//
//		if (DecalActor)
//		{
//			//   Į ũ    ȸ       
//			FVector DecalScale = FVector(10.0f, 10.0f, 10.0f); //   Į ũ       
//			DecalActor->SetActorScale3D(DecalScale);
//
//			//   Į          Ƽ         
//			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, DecalActor);
//			DecalActor->SetDecalMaterial(DynamicMaterial);
//
//			//   Į       ʱ ȭ
//			DecalActor->AttachToComponent(me->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
//			DecalActor->RegisterAllComponents();
//		} 
//	}
//}
void UPlayerFireComponent::PrintLog()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("FireComp")), true, FVector2D(1, 1));
	const FString localRoleString = UEnum::GetValueAsString<ENetRole>(myLocalRole);
	const FString remoteRoleString = UEnum::GetValueAsString<ENetRole>(myRemoteRole);
	const FString ownerString = GetOwner() != nullptr ? GetOwner()->GetName() : FString("No Owner");
	//const FString connectionString = GetNetConnection() ! = nullptr ? FString("Valid Connection") : FString("Invalid Connection");
	//const FString printString = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nOwner Name: %s\nNet Connection : %s"), *localRoleString, *remoteRoleString, *ownerString, *connectionString);
	const FString printString = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nOwner Name: %s\nNet Connection : %s"), *localRoleString, *remoteRoleString, *ownerString);

	//DrawDebugString(GetWorld(), me->GetActorLocation()+FVector::UpVector*2, printString, nullptr, FColor::White, 0, true);
}

void UPlayerFireComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UPlayerFireComponent, timeTest);
	//DOREPLIFETIME_CONDITION(ANetworkTestCharacter, timeTest, COND_OwnerOnly);
	//DOREPLIFETIME(ANetworkTestCharacter, jumpCount);
	DOREPLIFETIME(UPlayerFireComponent, owningWeapon);
	DOREPLIFETIME(UPlayerFireComponent, ammo);
	DOREPLIFETIME(UPlayerFireComponent, attackPower);
	DOREPLIFETIME(UPlayerFireComponent, fireInterval);
	//DOREPLIFETIME(UPlayerFireComponent, me->CurrHP);
	//DOREPLIFETIME(ANetworkTestCharacter, myName);
	//DOREPLIFETIME(ANetworkTestCharacter, myMeshNumber);
	//DOREPLIFETIME(ANetworkTestCharacter, myMeshColor);
}