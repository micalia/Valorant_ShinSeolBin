// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "BaseCharacter.h"
#include "PlayerFireComponent.h"
#include <Engine/SkeletalMeshSocket.h>
#include <Particles/ParticleSystemComponent.h>
#include <Engine/SkeletalMesh.h>
#include "Net/UnrealNetwork.h"
#include "PlayerBaseComponent.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetSimulatePhysics(false);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	meshComp->SetupAttachment(GetRootComponent());
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempWeaponMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Marketplace/MilitaryWeapDark/Weapons/Sniper_Rifle_B.Sniper_Rifle_B'"));
	if (tempWeaponMesh.Succeeded()) {
		meshComp->SetSkeletalMesh(tempWeaponMesh.Object);
	}

	MuzzleFlashComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashComp"));
	MuzzleFlashComp->SetupAttachment(meshComp, TEXT("FirePos"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> tempMuzzleFlash(TEXT("/Script/Engine.ParticleSystem'/Game/PSH/MilitaryWeapSilver/FX/P_Pistol_MuzzleFlash_01.P_Pistol_MuzzleFlash_01'"));
	if (tempMuzzleFlash.Succeeded()) {
		MuzzleFlash = tempMuzzleFlash.Object;
	}

	MuzzleFlashComp->SetTemplate(MuzzleFlash);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempReloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Gun/AM_SniperReload.AM_SniperReload'"));
	if (tempReloadMontage.Succeeded()) {
		ReloadMontage = tempReloadMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempSniperFireMontage(TEXT("/Script/Engine.AnimMontage'/Game/Marketplace/MilitaryWeapDark/Weapons/Anims/AM_SniperFire.AM_SniperFire'"));
	if (tempSniperFireMontage.Succeeded()) {
		SniperFireMontage = tempSniperFireMontage.Object;
	}

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	playerInstance = Cast<ABaseCharacter>(GetAttachParentActor());
	if (playerInstance) {
		fireComp = Cast<UPlayerFireComponent>(playerInstance->GetComponentByClass(UPlayerFireComponent::StaticClass()));
	}
	SetOwner(playerInstance);
	GrabWeapon(playerInstance); // ÃÑ ÀåÂø, ÃÑ¾Ë °¹¼ö ³Ñ±è
}
// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//¼¼ÆÃ, Å¬¶ó
void ABaseWeapon::GrabWeapon(ABaseCharacter* player)
{
	if (player) {
		if (fireComp) {
			fireComp->SetOwningWeapon(this);

			// ÃÑÀÇ Á¤º¸ ³Ñ±â±â
			fireComp->SetAmmo(ammo); //ÃÑ¾Ë
			fireComp->SetAttckPower(attackPower); //µ¥¹ÌÁö
		}

		boxComp->SetSimulatePhysics(false);
		FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

		if (player != nullptr)
		{
			//// ¹ß»ç Fire Location
			boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
			boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
}