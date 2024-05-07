// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_Neon_RelayBolt.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SH_Neon.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "PSH_Neon_RelayBolt_Boom.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APSH_Neon_RelayBolt::APSH_Neon_RelayBolt()
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
	compMesh->SetCollisionProfileName("volt");

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->UpdatedComponent = RootComponent;
	ProjectileComp->InitialSpeed = power;
	ProjectileComp->MaxSpeed = power;
	ProjectileComp->bRotationFollowsVelocity = false;
	ProjectileComp->bShouldBounce = false;
	Dir = GetActorForwardVector();

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APSH_Neon_RelayBolt::BeginPlay()
{
	Super::BeginPlay();
	compShphere->OnComponentHit.AddDynamic(this, &APSH_Neon_RelayBolt::OnComponentHit);


	
	/*ProjectileComp->Velocity = Dir * power;*/
}

// Called every frame
void APSH_Neon_RelayBolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APSH_Neon_RelayBolt::SeverShotBolt_Implementation(class ASH_Neon* neon)
{

}

void APSH_Neon_RelayBolt::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto wall = OtherComp->GetCollisionObjectType();
	FVector startLoc = GetActorLocation();
	FVector endLoc = startLoc + FVector::DownVector * 5000;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	FHitResult hitInfo;
	ASH_Neon* player = Cast<ASH_Neon>(OtherActor);
	
	if (player)
	{
		return;
	}
		
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, params))
	{
			UE_LOG(LogTemp,Warning,TEXT("%s"),*hitInfo.GetActor()->GetName());
			
			FActorSpawnParameters parm;
			parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// 타이머 핸들을 통해 소환 을 0.5초 에 1번으로 지정
			FTimerHandle SpawnTimer;
		
			GetWorld()->SpawnActor<APSH_Neon_RelayBolt_Boom>(boomFat, hitInfo.ImpactPoint, FRotator::ZeroRotator, parm);

			
// 			
// 			if (GetWorld()->SpawnActor<APSH_Neon_RelayBolt_Boom>(boomFat, hitInfo.ImpactPoint,FRotator::ZeroRotator,parm))
// 			{
// 				
// 			}	
	
	}
	//DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Red, false, 2, 0, 10);
	ProjectileComp->bShouldBounce = true;
	destroyNum++;
	SetLifeSpan(5.0f);
	if (destroyNum >= 2)
	{
		Destroy();
	}
	
	
	
// 	FVector ReflectDirection = FMath::GetReflectionVector(GetActorForwardVector(), Hit.Normal);
// 	FVector Impulse = ReflectDirection * power;
// 	OtherComp->AddImpulse(Impulse, NAME_None, true);
	// 벽이면 반사각으로 날아감 구현


	
}

