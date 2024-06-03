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

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempWeaponMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SK_KA_Val_X.SK_KA_Val_X'"));
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
		fireComp->SetOwningWeapon(this);

		// ���� ���� �ѱ��
		fireComp->SetAmmo(ammo); //�Ѿ�
		fireComp->SetAttckPower(attackPower); //������
		fireComp->SetFireInterval(fireInterval); //�Ѿ� ������

		fireComp->Setmagazine(magazine); //źâ����

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