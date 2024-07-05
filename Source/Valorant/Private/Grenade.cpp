// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Public/SB_Sova.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "BaseCharacter.h"
#include "../../Engine/Classes/Sound/SoundCue.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	RootComp->SetSphereRadius(15);
	RootComp->SetWorldScale3D(FVector(1.5));
	RootComp->SetCollisionProfileName(TEXT("NoCollision"));
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplosionEffect(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplosionEffect.Succeeded()) {
		ExplosionEffect = tempExplosionEffect.Object;
	}

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

	ProjectileComp->SetIsReplicated(true);
	ProjectileComp->bAutoActivate = false;
	bReplicates = true;
	SetReplicateMovement(true);

	static ConstructorHelpers::FObjectFinder<USoundCue> tempExplosionSound(TEXT("/Script/Engine.SoundCue'/Game/SB/Sounds/Grenade/SC_GrenadeExplosion.SC_GrenadeExplosion'"));
	if (tempExplosionSound.Succeeded()) {
		ExplosionSound = tempExplosionSound.Object;
	}
	
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

	RootComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Throw(FVector Velocity)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileComp->Velocity = Velocity;
	ProjectileComp->Activate(false);
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]()->void {
		#pragma region SimulatePhysics
			RootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MyVelocity = GetVelocity();
			if (HasAuthority()) {
				RootComp->SetSimulatePhysics(true);
				RootComp->SetPhysicsLinearVelocity(MyVelocity, false);
			}
		#pragma endregion

		}), 0.3f, false);

	FRotator RanRot = UKismetMathLibrary::RandomRotator(false);
	FLatentActionInfo LatentInfo;
	UKismetSystemLibrary::MoveComponentTo(RootComp, FVector(0), RanRot, false, false, 0.5f, false, EMoveComponentAction::Move, LatentInfo);
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHitCheck) {
		bHitCheck = true;
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]()->void {
			if (HasAuthority()) {
				ServerExplosion_Implementation();
			}
		}), 0.7f, false);
	}
}

void AGrenade::ServerExplosion_Implementation()
{
	MulticastExplosion();
}

void AGrenade::MulticastExplosion_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), FVector(7.5));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	TArray<FHitResult> OutHits;
	TArray<AActor*> EmptyActorsToIgnore;
	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		ExplosionRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel9),
		true,
		EmptyActorsToIgnore,
		EDrawDebugTrace::None,
		OutHits,
		true
	);

	if (bResult) {
		for (auto& Element : OutHits)
		{
			if (ABaseCharacter* DamagedPlayer = Cast<ABaseCharacter>(Element.GetActor())) {
				if (ABaseCharacter* GrenadeOwner = Cast<ABaseCharacter>(GetOwner())) {
					DamagedPlayer->ServerDamagedHealth(ExplosionDamage, GrenadeOwner);
				}
			}
		}
	}

	GrenadeMeshComp->SetHiddenInGame(true);
	SetLifeSpan(3);
}

void AGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGrenade, bHitCheck);
}