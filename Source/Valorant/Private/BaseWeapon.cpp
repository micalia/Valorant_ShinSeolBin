// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "BaseCharacter.h"
#include "PlayerFireComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include <Engine/SkeletalMesh.h>

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

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempMuzzleFlash(TEXT("/Script/Engine.ParticleSystem'/Game/PSH/MilitaryWeapSilver/FX/P_Pistol_MuzzleFlash_01.P_Pistol_MuzzleFlash_01'"));
	if (tempMuzzleFlash.Succeeded()) {
		MuzzleFlash = tempMuzzleFlash.Object;
	}

	MuzzleFlashComp->SetTemplate(MuzzleFlash);

	 ConstructorHelpers::FObjectFinder<UAnimMontage> tempReloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Gun/AM_SniperReload.AM_SniperReload'"));
	if (tempReloadMontage.Succeeded()) {
		ReloadMontage = tempReloadMontage.Object;
	}

	 ConstructorHelpers::FObjectFinder<UAnimMontage> tempSniperFireMontage(TEXT("/Script/Engine.AnimMontage'/Game/Marketplace/MilitaryWeapDark/Weapons/Anims/AM_SniperFire.AM_SniperFire'"));
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
		if (!playerInstance->IsLocallyControlled()) {
			meshComp->SetRenderCustomDepth(true);
			meshComp->CustomDepthStencilValue = 1;
		}
	}
	SetOwner(playerInstance);
	GrabWeapon(playerInstance); // �� ����, �Ѿ� ���� �ѱ�
}
// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//����, Ŭ��
void ABaseWeapon::GrabWeapon(ABaseCharacter* player)
{
	if (player) {
		if (fireComp) {
			fireComp->SetOwningWeapon(this);

			// ���� ���� �ѱ��
			fireComp->SetAmmo(ammo); //�Ѿ�
			fireComp->SetAttckPower(attackPower); //������
		}

		boxComp->SetSimulatePhysics(false);
		FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

		if (player != nullptr)
		{
			//// �߻� Fire Location
			boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
			boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
}