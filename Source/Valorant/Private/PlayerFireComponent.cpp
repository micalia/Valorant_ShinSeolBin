// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "BaseCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <TimerManager.h>
#include <Camera/CameraComponent.h>
#include <Camera/CameraShakeBase.h>
#include <Engine/SkeletalMeshSocket.h>
#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include <DrawDebugHelpers.h>
#include <Components/ActorComponent.h>
#include "Components/SceneComponent.h"
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "GameFramework/Character.h"
#include <Components/SkeletalMeshComponent.h>
#include "SB_Sova.h"
#include "SB_SovaAnim.h"
#include "Sound/SoundCue.h"
#include "SkillWidget.h"
#include "Components/TextBlock.h"

UPlayerFireComponent::UPlayerFireComponent()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempFireMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Gun/AM_NewFire.AM_NewFire'"));
	if (tempFireMontage.Succeeded()) {
		FireMontage = tempFireMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempReloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Gun/AM_Reload.AM_Reload'"));
	if (tempReloadMontage.Succeeded()) {
		ReloadMontage = tempReloadMontage.Object;
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

	static ConstructorHelpers::FObjectFinder<USoundCue> tempSniperSound(TEXT("/Script/Engine.SoundCue'/Game/SB/Sounds/Kar98_Cue.Kar98_Cue'"));
	if (tempSniperSound.Succeeded()) {
		SniperSound = tempSniperSound.Object;
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
	if(me->TheEndGame) return;
	if (mySova == nullptr) {
		mySova = Cast<ASB_Sova>(me);
	}
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		if (mySova != nullptr && mySova->GetCurrState() == ESovaState::DefaultAtk && mySova->GetSniperOn() == false) {
			Fire();
		}
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
	bReloadOn = true;
	mySova->Server_SetCurrState(ESovaState::Reload);
	if (me->HasAuthority()) {
		Server_Reload_Implementation();
	}
	else {
		Server_Reload();
	}
}

void UPlayerFireComponent::Server_Reload_Implementation()
{
	bReloadOn = true;
	Multicast_Reload(FullAmmoCount);
}

void UPlayerFireComponent::Multicast_Reload_Implementation(int32 BulletCount)
{
	USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
	if (SovaFpsAnim) {
		SovaFpsAnim->StopAllMontages(0.0f);
	}

	if (owningWeapon) {
		if (auto GunAnim = owningWeapon->meshComp->GetAnimInstance()) {
			GunAnim->Montage_Play(owningWeapon->ReloadMontage);
		}
	}

	if (ASB_Sova* Sova = Cast<ASB_Sova>(me)) {
		if (me->IsLocallyControlled()) {
			Sova->skillWigetInstance->AmmoCnt_txt->SetText(FText::AsNumber(BulletCount));
			Sova->MouseRightReleasedAction();
		}
		if (SovaFpsAnim) {
			if (me->HasAuthority()) {
				SovaFpsAnim->StopAllMontages(0.0f);
				SovaFpsAnim->Montage_Play(ReloadMontage);
			}
			else {
				SovaFpsAnim->StopAllMontages(0.0f);
				SovaFpsAnim->Montage_Play(ReloadMontage);
			}
		}
	}
	if (ReloadMontage) {
		me->PlayAnimMontage(ReloadMontage);
	}

}

void UPlayerFireComponent::ServerReloadComplete_Implementation()
{
	bReloadOn = false;
	ammo = FullAmmoCount;
	MulticastReloadComplete(ammo);
}

void UPlayerFireComponent::MulticastReloadComplete_Implementation(int32 AmmoCnt)
{
	if (me && me->IsLocallyControlled()) {
		OnAmmoCntChanged.Broadcast(AmmoCnt);
		bReloadOn = false;
	}
}

void UPlayerFireComponent::Fire()
{
	if (me->TheEndGame) return;
	if (owningWeapon == nullptr)
		return;
	if (!me->IsLocallyControlled() || bReloadOn)return;

	if (bInDelay)
	{
		return;
	}

	me->bCameraShake = true;
	me->GetWorldTimerManager().ClearTimer(fireDelay);
	me->GetWorldTimerManager().SetTimer(fireDelay, FTimerDelegate::CreateLambda([&]() {
		bInDelay = false;
	}), FireDelayTime, false);

	//RPC             
	if (bInDelay == false) {
		if (me->HasAuthority())
		{
			ServerFire_Implementation();
		}
		else
		{
			ServerFire();
		}
		bInDelay = true;
	}
}

void UPlayerFireComponent::ServerFire_Implementation()
{
	if (bReloadOn) return;
	if (ammo > 0)
	{
		ammo--;

		FVector startLoc = me->FollowCamera->GetComponentLocation();
		FVector endLoc = startLoc + me->FollowCamera->GetForwardVector() * 9000;
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

		bool bReloadChk;
		if (ammo <= 0) {
			bReloadChk = true;
			mySova->ChangeCurrState(ESovaState::Reload);
		}
		else {
			bReloadChk = false;
		}
		MulticastFire(bReloadChk, ammo);
		if (bReloadChk) Reload();
	}
}

bool UPlayerFireComponent::ServerFire_Validate()
{
	return true;
}

void UPlayerFireComponent::MulticastFire_Implementation(bool InFireReloadChk, int32 CurrAmmoCnt)
{
	if (!InFireReloadChk && FireMontage)
	{
		me->PlayAnimMontage(FireMontage);
		USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
		SovaFpsAnim->Montage_Play(FireMontage);
	}
	if (me) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, me->GetActorLocation(), 1.3f);
	}
	OnAmmoCntChanged.Broadcast(CurrAmmoCnt);
}

void UPlayerFireComponent::MulticastSniperShot_Implementation(bool InReloadOn, int32 CurrAmmoCnt)
{
	if (!InReloadOn && FireMontage)
	{
		me->PlayAnimMontage(FireMontage);
		USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
		SovaFpsAnim->Montage_Play(FireMontage);
	}
	if (ensure(me)) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SniperSound, me->GetActorLocation(), 0.7f);
	}
	OnAmmoCntChanged.Broadcast(CurrAmmoCnt);
}

void UPlayerFireComponent::ServerFireEffect_Implementation(class ABaseWeapon* Gun, const USkeletalMeshSocket* FireSocket, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (Gun)
	{
		MulticastFireEffect(Gun, p1, p2, p3, p4, bBlockingHit);
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
}

void UPlayerFireComponent::ServerSniperEffect_Implementation(class ABaseWeapon* Gun, const USkeletalMeshSocket* FireSocket, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (Gun)
	{
		MulticastSniperEffect(Gun, p1, p2, p3, p4, bBlockingHit);
	}
}

void UPlayerFireComponent::MulticastSniperEffect_Implementation(class ABaseWeapon* Gun, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit)
{
	if (Gun) {
		if (UAnimInstance* GunAnim = Gun->meshComp->GetAnimInstance()) {
			GunAnim->Montage_Play(Gun->SniperFireMontage);
		}
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
	isFire = false;
}

void UPlayerFireComponent::ServerStopFire_Implementation()
{
	isFire = false;
}

void UPlayerFireComponent::SetAmmoCountTextInit(USkillWidget* SkillUI)
{
	if (SkillUI) {
		OnAmmoCntChanged.AddUObject(SkillUI, &USkillWidget::SetAmmoCount);
		SkillUI->AmmoCnt_txt->SetText(FText::AsNumber(GetAmmo()));
	}
}

void UPlayerFireComponent::SniperShot()
{
	if (me->TheEndGame) return;
	if (!me->IsLocallyControlled() || bReloadOn)return;

	if (bInSniperDelay)
	{
		return;
	}

	me->GetWorldTimerManager().ClearTimer(SniperDelay);
	me->GetWorldTimerManager().SetTimer(SniperDelay, FTimerDelegate::CreateLambda([&]() {
		bInSniperDelay = false;
	}), SniperDelayTime, false);

	//RPC             
	if (bInSniperDelay == false) {
		if (me->HasAuthority()) {
			ServerSniperShot_Implementation();
		}
		else {
			ServerSniperShot();
		}

		if (me->IsLocallyControlled()) {
			if (ASB_Sova* Sova = Cast<ASB_Sova>(me)) {
				Sova->SniperRecoilCameraEffect();
			}
		}
		bInSniperDelay = true;
	}

}

void UPlayerFireComponent::ServerSniperShot_Implementation()
{
	if (ammo > 0)
	{
		ammo--;

		FVector startLoc = me->FollowCamera->GetComponentLocation();
		FVector endLoc = startLoc + me->FollowCamera->GetForwardVector() * 22000;
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
				hitPawn->ServerDamagedHealth(SniperPower, me);
				ServerHitProcess();
			}

		}
		ServerSniperEffect_Implementation(owningWeapon, FireSocket, SocketTransform.GetLocation(), hitInfo.ImpactPoint, hitInfo.ImpactNormal.Rotation(), endLoc, hitInfo.bBlockingHit);
		
		/*if (me->HasAuthority()) {
			me->GetWorldTimerManager().ClearTimer(fireDelay);
			me->GetWorldTimerManager().SetTimer(fireDelay, FTimerDelegate::CreateLambda([&]() {
				bInDelay = !bInDelay;
				}), fireInterval, false);
		}*/

		bool ReloadChk;
		if (ammo <= 0) {
			ReloadChk = true;
			mySova->ChangeCurrState(ESovaState::Reload);
			
		}
		else {
			ReloadChk = false;
		}
		MulticastSniperShot(ReloadChk, ammo);
		if (ReloadChk)Reload();
	}
}

void UPlayerFireComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerFireComponent, owningWeapon);
	DOREPLIFETIME(UPlayerFireComponent, ammo);
	DOREPLIFETIME(UPlayerFireComponent, attackPower);
	DOREPLIFETIME(UPlayerFireComponent, bReloadOn);
	DOREPLIFETIME(UPlayerFireComponent, isFire);
}
