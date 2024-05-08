// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_Arrow.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Net/UnrealNetwork.h"

// Sets default values
ASB_Arrow::ASB_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileComp"));

	bReplicates = true;
	SetReplicateMovement(true);
}

void ASB_Arrow::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("PreInit"))
	projectileComp->InitialSpeed = initSpeed;
	projectileComp->MaxSpeed = initSpeed;
}

void ASB_Arrow::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
		UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents : %f"), projectileComp->InitialSpeed)
}

// Called when the game starts or when spawned
void ASB_Arrow::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay : %f"), projectileComp->InitialSpeed)
}

// Called every frame
void ASB_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASB_Arrow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_Arrow, currBounceCount);
}