// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_HitTestPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASH_HitTestPlayer::ASH_HitTestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/PSH/Resouce/Neon/bvgfv7667.bvgfv7667'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -85));
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
}

// Called when the game starts or when spawned
void ASH_HitTestPlayer::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
	
}

// Called every frame
void ASH_HitTestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (health <= 0)
	{
		DieProcess();
		Destroy();
	}
}

// Called to bind functionality to input
void ASH_HitTestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASH_HitTestPlayer::HitProcess()
{
	PlayAnimMontage(hitMontage);
// 	UGameplayStatics::SpawnEmitterAtLocation(
// 		GetWorld(),      // 현재 월드
// 		hitParticle, // 생성할 파티클 시스템
// 		GetActorLocation(),  // 시작점 위치
// 		FRotator::ZeroRotator,
// 		true);
}

void ASH_HitTestPlayer::DamagedHealth(int32 value)
{
	health = FMath::Max(health - value, 0);
}	

void ASH_HitTestPlayer::DieProcess()
{

}

