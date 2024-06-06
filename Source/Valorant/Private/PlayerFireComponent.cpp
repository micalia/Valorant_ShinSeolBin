// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "BaseCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "FireUserWidget.h"
#include <TimerManager.h>
#include <Camera/CameraComponent.h>
#include <Engine/DecalActor.h>
#include <Camera/CameraShakeBase.h>
#include "Components/SizeBox.h"
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
#include "SB_SovaAnim.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "SkillWidget.h"
#include "Components/TextBlock.h"

UPlayerFireComponent::UPlayerFireComponent()
{
	static ConstructorHelpers::FClassFinder<UFireUserWidget> tempFireWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WBP_Fire.WBP_Fire_C'"));
	if (tempFireWidget.Succeeded()) {
		fireWidget = tempFireWidget.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempFireMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Gun/AM_NewFire.AM_NewFire'"));
	if (tempFireMontage.Succeeded()) {
		FireMontage = tempFireMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> tempImpactParticles(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/GunBulletEffect.GunBulletEffect'"));
	if (tempImpactParticles.Succeeded()) {
		ImpactParticles = tempImpactParticles.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> tempMuzzleFlash(TEXT("/Script/Engine.ParticleSystem'/Game/PSH/MilitaryWeapSilver/FX/P_Pistol_MuzzleFlash_01.P_Pistol_MuzzleFlash_01'"));
	if (tempMuzzleFlash.Succeeded()) {
		MuzzleFlash = tempMuzzleFlash.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> tempFireSound(TEXT("/Script/Engine.SoundCue'/Game/SB/Sounds/Gun/SC_SovaGun.SC_SovaGun'"));
	if (tempFireSound.Succeeded()) {
		FireSound = tempFireSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> tempBeamParticles(TEXT("/Script/Engine.ParticleSystem'/Game/PSH/TrilFX/SmokeBeam/P_SmokeTrail.P_SmokeTrail'"));
	if (tempBeamParticles.Succeeded()) {
		BeamParticles = tempBeamParticles.Object;
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
	if (isFire)//isFire
	{
		Fire();	
	}
}

void UPlayerFireComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(me->Key_X_action, ETriggerEvent::Started, this, &UPlayerFireComponent::Reload);
	}
}

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
		OnAmmoCntChanged.Broadcast(ammo);
		//me->OnAmmoCntChanged.Broadcast(ammo);
		FVector startLoc = me->FollowCamera->GetComponentLocation();
		FVector endLoc = startLoc + me->FollowCamera->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		
		params.AddIgnoredActor(me);

		//---------effect-----------
		const USkeletalMeshSocket* FireSocket = owningWeapon->meshComp->GetSocketByName("FirePos");

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
		ServerFireEffect(owningWeapon, FireSocket, SocketTransform.GetLocation(), hitInfo.ImpactPoint, hitInfo.ImpactNormal.Rotation(), endLoc, hitInfo.bBlockingHit);
		//          
		GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Fire"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));

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
	if (FireMontage)
	{	
		me->PlayAnimMontage(FireMontage);
		USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
		SovaFpsAnim->Montage_Play(FireMontage);
		me->PlayFpsMontage();
	}
}

void UPlayerFireComponent::ServerFireEffect_Implementation(class ABaseWeapon* Gun, const USkeletalMeshSocket* FireSocket, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (Gun)
	{
		MulticastFireEffect(Gun, p1,p2,p3,p4,bBlockingHit);
	}
}

void UPlayerFireComponent::MulticastFireEffect_Implementation(class ABaseWeapon* Gun, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (Gun) {
		Gun->MuzzleFlashComp->Activate(true);
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactParticles,
				p2,
				p3,
				FVector(0.35)
			);
		}
	}
	/*if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleFlash,
			p1, FRotator(0,0,90)
		);
	}*/
	//    
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	FVector BeamEnd = p4;
	//	if (bBlockingHit)
	//	{
	//		BeamEnd = p2;
	//	}
	//	if (BeamParticles)
	//	{
	//		//  Ƽĳ  Ʈ
	//		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
	//			World,
	//			BeamParticles,
	//			p1,
	//			FRotator::ZeroRotator,
	//			true
	//		);
	//		if (Beam)
	//		{
	//			Beam->SetVectorParameter(FName("Target"), BeamEnd);
	//		}
	//	}
	//}
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
			if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController()) {
				isFire = false;
				GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Stop FIre"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
				//UI cross hair
				sova->fire_UI->isFire = false;
			}
		}
	}
	
	if (playerController)
	{
		curRightRecoilAmount = 0;
		curRecoilAmount = 0;
	}
}

void UPlayerFireComponent::SetAmmoCountTextInit(USkillWidget* SkillUI)
{
	if (SkillUI) {
		OnAmmoCntChanged.AddUObject(SkillUI, &USkillWidget::SetAmmoCount);
		SkillUI->AmmoCnt_txt->SetText(FText::AsNumber(GetAmmo()));
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
