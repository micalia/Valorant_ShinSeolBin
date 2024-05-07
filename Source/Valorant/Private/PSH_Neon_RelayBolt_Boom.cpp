// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_Neon_RelayBolt_Boom.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "SH_RelayBolt_OverlapActor.h"

// Sets default values
APSH_Neon_RelayBolt_Boom::APSH_Neon_RelayBolt_Boom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compShphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = compShphere;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempShphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (tempShphere.Succeeded())
	{
		compMesh->SetStaticMesh(tempShphere.Object);
	}

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APSH_Neon_RelayBolt_Boom::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APSH_Neon_RelayBolt_Boom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (setSize < 10)
	{
	setSize += DeltaTime * 30;
	SetActorScale3D(FVector(setSize));
	}

	if (setSize > 10) // fx 가 원을 가득 채웠을 시간초로 변환
	{
		if (OnesNia)
		{
			return;
		}
		
		OnesNia = true;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Ring, GetActorLocation());
		FTimerHandle spawnTimer;
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(spawnTimer,FTimerDelegate::CreateLambda([&](){
// 			UGameplayStatics::SpawnEmitterAtLocation(
// 				GetWorld(),      // 현재 월드
// 				ExParticle, // 생성할 파티클 시스템
// 				GetActorLocation(),  // 시작점 위치
// 				FRotator::ZeroRotator, // 회전값 (여기서는 회전하지 않음)
// 				true            // 파티클 시스템을 소유하는 액터의 생명주기에 따라 파괴 여부 (true면 액터와 함께 파괴됨)
// 			);
			FVector spawnLoc = GetActorLocation();
			spawnLoc.Z+=300;
						
			FActorSpawnParameters parm;
			parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<ASH_RelayBolt_OverlapActor>(reboltFat,spawnLoc, FRotator::ZeroRotator, parm);

		}),1.0f,false);

		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, FTimerDelegate::CreateLambda([&]() {
			Destroy();
			}), 1.0f, false);

		/*
		* me = Cast<ABaseCharacter>(GetOwner());
		ABaseCharacter* hitPawn = Cast<ABaseCharacter>(otherActor or otherActor.GetActor());
			hitPawn->ServerDamagedHealth(attackPower, me);
		*/

	}
}


