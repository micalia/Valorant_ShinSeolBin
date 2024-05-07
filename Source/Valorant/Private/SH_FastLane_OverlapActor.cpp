// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_FastLane_OverlapActor.h"
#include "Components/BoxComponent.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>

// Sets default values
ASH_FastLane_OverlapActor::ASH_FastLane_OverlapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = compBox;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(RootComponent);
// 	//compMesh->SetRelativeLocation(FVector())

// 	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh (TEXT("/Script/Engine.StaticMesh'/Game/thunder/ksh_effect/wall/123.123'"));
// 	if (tempMesh.Succeeded())
// 	{
// 		compMesh->SetStaticMesh(tempMesh.Object);
// 	}
// 	bReplicates = true;
}

// Called when the game starts or when spawned
void ASH_FastLane_OverlapActor::BeginPlay()
{
	Super::BeginPlay();

	/*demageBox->*/
	/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), spake, compMesh->GetRelativeLocation(), FRotator(0, 90, 0));*/
}

// Called every frame
void ASH_FastLane_OverlapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	if (TimeOn)
// 	{
// 		CurTime+=DeltaTime;
// 	}
// 
// 	if (CurTime >= 7)
// 	{
// 		ScalUp = false;
// 	}
// 	
// 	if (ScalUp)
// 	{
// 		if (Scal <= 4)
// 		{
// 			
// 		Scal+= 3* DeltaTime;
// 		compMesh->SetRelativeScale3D(FVector(1,2,Scal));
// 		}
// 	}
// 	else 
// 	{
// 		Scal_Z -= 2*DeltaTime;
// 		compMesh->SetRelativeScale3D(FVector(1, Scal_Z, Scal));
// 		if (Scal_Z <= 0)
// 		{
// 			Destroy();
// 		}
// 	}

}

