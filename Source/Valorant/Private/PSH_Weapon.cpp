// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SH_Neon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values


APSH_Weapon::APSH_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetSimulatePhysics(true);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	meshComp->SetupAttachment(GetRootComponent());
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	
}

// Called when the game starts or when spawned
void APSH_Weapon::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APSH_Weapon::OnOverlap);

	ASH_Neon* player = Cast<ASH_Neon>(UGameplayStatics::GetActorOfClass(GetWorld(), ASH_Neon::StaticClass()));
	SetOwner(player);
	GrabWeapon(player);

}

void APSH_Weapon::ReleaseWeapon(class ASH_Neon* player)
{
	ammo = player->GetAmmo();
	player->WeaponInfoReset();
	player->SetOwningWeapon(nullptr);

	FDetachmentTransformRules rules = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromActor(rules);
	boxComp->SetSimulatePhysics(true);

	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FTimerHandle colTimer;
	GetWorldTimerManager().SetTimer(colTimer, FTimerDelegate::CreateLambda([&]() {
		boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		}), 3.0f, false);


}

// Called every frame
void APSH_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}


void APSH_Weapon::Reloading(class ASH_Neon* player)
{
	if (player->magazine > 0)
	{
		player->magazine--;
		player->ammo = 30;
	}
}

void APSH_Weapon::Fire(FHitResult& FireHit, const FVector& Start, const FVector& End)
{
	const USkeletalMeshSocket* FireSocket = meshComp->GetSocketByName("Fire Location");
	FTransform SocketTransform = FireSocket->GetSocketTransform(meshComp);
	FVector StartTl = SocketTransform.GetLocation();
	if (FireSocket)
	{	
		
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactParticles,
				FireHit.ImpactPoint,
				FireHit.ImpactNormal.Rotation()

			);
			UE_LOG(LogTemp, Warning, TEXT("impact"));
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				StartTl
			);
			UE_LOG(LogTemp, Warning, TEXT("flash"));
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation()
			);
			UE_LOG(LogTemp,Warning,TEXT("sound"));
		}
	}

	UWorld* World = GetWorld();
	if (World)
	{ 
		FVector BeamEnd = End;
		if (FireHit.bBlockingHit)
		{
			BeamEnd = FireHit.ImpactPoint;
		}
		else
		{
			FireHit.ImpactPoint = End;
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				StartTl,
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

void APSH_Weapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASH_Neon* player = Cast<ASH_Neon>(OtherActor);

	if (player != nullptr && player->GetOwningWeapon() == nullptr)
	{
		SetOwner(player);

		GrabWeapon(player);
	}
}

void APSH_Weapon::GrabWeapon(class ASH_Neon* player)
{
	player->SetOwningWeapon(this);

	// 총의 정보 넘기기
	player->SetAmmo(ammo);
	player->SetAttckPower(attackPower);
	player->SetFireInterval(fireInterval);
	player->Setmagazine(magazine);

	boxComp->SetSimulatePhysics(false);
	FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (player != nullptr)
	{
		AttachToComponent(player->GetMesh(), rules, FName("Weapon Loc"));
		// 발사 Fire Location
		boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 애니메이션 변경하기
		/*UPlayerAnimInstance* anim = Cast<UPlayerAnimInstance>(player->GetMesh()->GetAnimInstance());*/

// 		if (anim != nullptr)
// 		{
// 			anim->bHasPistol = true;
// 		}
	}
}

