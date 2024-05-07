// Fill out your copyright notice in the Description page of Project Settings.


#include "Left_NSH.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ALeft_NSH::ALeft_NSH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision=CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent=SphereCollision;

	SphereCollision->SetSphereRadius(5);
	SphereCollision->SetRelativeScale3D(FVector(1));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (TempStaticMesh.Succeeded())
	{
		Sphere->SetStaticMesh(TempStaticMesh.Object);
		Sphere->SetupAttachment(RootComponent);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetRelativeScale3D(FVector(0.1));
		//
	}

	
}

// Called when the game starts or when spawned
void ALeft_NSH::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeft_NSH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

