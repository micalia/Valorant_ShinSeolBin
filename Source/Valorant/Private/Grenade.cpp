// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Public/SB_Sova.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	RootComp->SetSphereRadius(15);
	RootComp->SetWorldScale3D(FVector(1.5));
	RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComp->SetIsReplicated(true);	

	GrenadeMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeMeshComp"));
	GrenadeMeshComp->SetupAttachment(RootComp);
	GrenadeMeshComp->SetRelativeLocation(FVector(0, 0, -7));
	GrenadeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGrenadeMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/cluster-grenade/source/grenade_skined.grenade_skined'"));
	if (tempGrenadeMesh.Succeeded()) {
		GrenadeMeshComp->SetSkeletalMesh(tempGrenadeMesh.Object);
	}

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	ProjectileComp->SetIsReplicated(true);
	ProjectileComp->bAutoActivate = false;
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	SovaOwner = Cast<ASB_Sova>(GetOwner());
	if (SovaOwner) {
		FAttachmentTransformRules AttachRule = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		if (SovaOwner->IsLocallyControlled()) {
			AttachToComponent(SovaOwner->fpsMesh, AttachRule, TEXT("Grenade"));
		}
		else {
			AttachToComponent(SovaOwner->GetMesh(), AttachRule, TEXT("Grenade"));
		}
	}
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

