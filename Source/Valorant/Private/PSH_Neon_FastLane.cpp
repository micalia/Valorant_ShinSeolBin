// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_Neon_FastLane.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SH_Neon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "SH_FastLane_OverlapActor.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


// Sets default values
APSH_Neon_FastLane::APSH_Neon_FastLane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	compShphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = compShphere;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempShphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempShphere.Succeeded())
	{
		compMesh->SetStaticMesh(tempShphere.Object);
	}
	compShphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->UpdatedComponent = RootComponent;
	ProjectileComp->InitialSpeed = power;
	ProjectileComp->MaxSpeed = power;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->ProjectileGravityScale = 0.0f;
	ProjectileComp->bShouldBounce = false;
	Dir = GetActorForwardVector();

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APSH_Neon_FastLane::BeginPlay()
{
	Super::BeginPlay();
	compShphere->OnComponentHit.AddDynamic(this, &APSH_Neon_FastLane::OnComponentHit);

}

// Called every frame
void APSH_Neon_FastLane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector leftDiagonaVector(-1.0f, -1.0f, -1.0f);
	FVector rightDiagonaVector(-1.0f, 1.0f, -1.0f);
// 	lefStartLoc = GetActorLocation() + leftDiagonaVector * 200;
// 	
// 	AdjustParticleLength(lefStartLoc,leftEndLoc);
// 	rightStartLoc = GetActorLocation() + rightDiagonaVector * 200;
// 	AdjustParticleLength(rightStartLoc,rightEndLoc);
	
	FVector curLoc = GetActorLocation();
	float distanceMoved = FVector::Distance(Start,curLoc);

	TotalDistanceMoved = distanceMoved;

	if (TotalDistanceMoved >= DistanceToMove)
	{
		Destroy();

	}
	// 이동 거리 확인
// 	if (Start >= endLoc)
// 	{
// 		Destroy();
// 	}
	
}


void APSH_Neon_FastLane::AdjustParticleLength(const FVector& StartLocation, const FVector& EndLocation)
{

}

void APSH_Neon_FastLane::TestLine(FVector& StartLocation, FVector& EndLocation)
{
	DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,2,0,10);

}

void APSH_Neon_FastLane::SpawnWall( FVector& StartLocation)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ASH_FastLane_OverlapActor>(fastLane_OverlapActorFat,StartLocation,FRotator::ZeroRotator,params);
// 	StartLocation.Y += -1250;
// 	StartLocation.X += 1200;
// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), spake, StartLocation,FRotator(0,90,0));
}

void APSH_Neon_FastLane::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASH_Neon* player = Cast<ASH_Neon>(OtherActor);
	if (player)
	{
		return;
	}


// 	if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, params))
// 	{
// 			
// 	}
}

