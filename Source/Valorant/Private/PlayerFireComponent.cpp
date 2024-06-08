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
	if (isFire)
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
	ammo = 5;
	bReloadOn = true;
	Multicast_Reload(ammo);
}

void UPlayerFireComponent::Multicast_Reload_Implementation(int32 BulletCount)
{
	USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
	if (SovaFpsAnim) {
		SovaFpsAnim->StopAllMontages(0.0f);
	}

	if (ReloadMontage) {
		me->PlayAnimMontage(ReloadMontage);
		if (owningWeapon) {
			if (auto GunAnim = owningWeapon->meshComp->GetAnimInstance()) {
				GunAnim->Montage_Play(owningWeapon->ReloadMontage);
			}
		}
	}
	if (me->IsLocallyControlled()) {
		if (ASB_Sova* Sova = Cast<ASB_Sova>(me)) {
			Sova->skillWigetInstance->AmmoCnt_txt->SetText(FText::AsNumber(BulletCount));
			Sova->MouseRightReleasedAction();
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
	}
}

void UPlayerFireComponent::ServerReloadComplete_Implementation()
{
	bReloadOn = false;
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
	if(bReloadOn) return;
	if (me->GetWorldTimerManager().IsTimerActive(fireDelay))
	{
		return;
	}
	if (ammo > 0)
	{
		ammo--;
		
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

		if (me->HasAuthority()) {
			me->GetWorldTimerManager().ClearTimer(fireDelay);
			me->GetWorldTimerManager().SetTimer(fireDelay, FTimerDelegate::CreateLambda([&]() {
				bInDelay = !bInDelay;
				}), fireInterval, false);
		}

		bool bReloadChk;
		if (ammo <= 0) {
			bReloadChk = true;
		}
		else {
			bReloadChk = false;
		}
		MulticastFire(bReloadChk);
		if(bReloadChk) Reload();
	}
}

bool UPlayerFireComponent::ServerFire_Validate()
{
	return true;
}

void UPlayerFireComponent::MulticastFire_Implementation(bool InFireReloadChk)
{
	if (!InFireReloadChk && FireMontage)
	{
		me->PlayAnimMontage(FireMontage);
		if (me->IsLocallyControlled()) {
			USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
			SovaFpsAnim->Montage_Play(FireMontage);
		}
	}
	if (me) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, me->GetActorLocation());
	}
	OnAmmoCntChanged.Broadcast(ammo);
}

void UPlayerFireComponent::MulticastSniperShot_Implementation(bool InReloadOn)
{
	if (!InReloadOn && FireMontage)
	{
		me->PlayAnimMontage(FireMontage);
		USB_SovaAnim* SovaFpsAnim = Cast<USB_SovaAnim>(me->fpsMesh->GetAnimInstance());
		SovaFpsAnim->Montage_Play(FireMontage);
	}
	if (me) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SniperSound, me->GetActorLocation());
	}
	OnAmmoCntChanged.Broadcast(ammo);
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
		/*if (UAnimInstance* GunAnim = Gun->meshComp->GetAnimInstance()) {
			GunAnim->Montage_Play(Gun->FireMontage);
		}*/
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
	if (me == nullptr) return;
	ASB_Sova* sova = Cast<ASB_Sova>(me);
	if (sova) {
		if (sova->fire_UI) {
			if (me->GetController() != nullptr && me->GetController()->IsLocalPlayerController()) {
				isFire = false;
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

void UPlayerFireComponent::SniperShot()
{
	
	if (me->IsLocallyControlled()) {
		if(ASB_Sova* Sova = Cast<ASB_Sova>(me)){
			Sova->SniperRecoilCameraEffect();
		}
	}

	if (me->HasAuthority()) {
		ServerSniperShot_Implementation();
	}
	else {
		ServerSniperShot();
	}
}

void UPlayerFireComponent::ServerSniperShot_Implementation()
{
	if (ammo > 0)
	{
		ammo--;

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

		if (me->HasAuthority()) {
			me->GetWorldTimerManager().ClearTimer(fireDelay);
			me->GetWorldTimerManager().SetTimer(fireDelay, FTimerDelegate::CreateLambda([&]() {
				bInDelay = !bInDelay;
				}), fireInterval, false);
		}

		bool ReloadChk;
		if (ammo <= 0) {
			ReloadChk = true;
		}
		else {
			ReloadChk = false;
		}
		MulticastSniperShot(ReloadChk);
		if(ReloadChk)Server_Reload_Implementation();
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
	DOREPLIFETIME(UPlayerFireComponent, bReloadOn);
}
