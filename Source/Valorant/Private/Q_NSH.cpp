// Fill out your copyright notice in the Description page of Project Settings.


#include "Q_NSH.h"
#include <Components/SphereComponent.h>

// Sets default values
AQ_NSH::AQ_NSH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->SetSphereRadius(150);
	SphereCollision->SetRelativeScale3D(FVector(1));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (TempStaticMesh.Succeeded())
	{
		Sphere->SetRelativeLocation(FVector(0, 0, 0));
		Sphere->SetStaticMesh(TempStaticMesh.Object);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetRelativeScale3D(FVector(0.1));
		Sphere->SetupAttachment(SphereCollision);
	}
}

// Called when the game starts or when spawned
void AQ_NSH::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3);
}

// Called every frame
void AQ_NSH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(DeltaTime*Speed*GetActorForwardVector()+GetActorLocation());
}

