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

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempWeaponMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/PSH/Resouce/Phantom2/Phantom.Phantom'"));
	if (tempWeaponMesh.Succeeded()) {
		meshComp->SetSkeletalMesh(tempWeaponMesh.Object);
	}
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	//boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnOverlap);
	
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
			//AttachToComponent(player->GetMesh(), rules, FName("WeaponLoc"));
			//// �߻� Fire Location
			boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
			boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// �ִϸ��̼� �����ϱ�
			/*UPlayerAnimInstance* anim = Cast<UPlayerAnimInstance>(player->GetMesh()->GetAnimInstance());*/

	// 		if (anim != nullptr)
	// 		{
	// 			anim->bHasPistol = true;
	// 		}
		}

	}
}

//void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(ABaseWeapon, ammo);
//}
