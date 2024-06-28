// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_DragonArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "SB_Sova.h"
#include "../../Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
ASB_DragonArrow::ASB_DragonArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	SMSovaArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMSovaArrow"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempArrowMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/arrow/DragonArrow/SovaDragonArrow.SovaDragonArrow'"));
	if (tempArrowMesh.Succeeded()) {
		SMSovaArrow->SetStaticMesh(tempArrowMesh.Object);
	}
	SMSovaArrow->SetupAttachment(RootComp);
	SMSovaArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SMSovaArrow->SetRelativeScale3D(FVector(2));
	SMSovaArrow->SetRelativeLocation(FVector(-163, 0, 0));
	SMSovaArrow->SetRelativeRotation(FRotator(0, 0, 0));
	SMSovaArrow->SetCastShadow(false);

	static ConstructorHelpers::FClassFinder<AActor> tempSparkEffectFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Effect/DragonArrow/BP_ArrowSparkEffect.BP_ArrowSparkEffect_C'"));
	if (tempSparkEffectFactory.Succeeded()) {
		SparkEffectFactory = tempSparkEffectFactory.Class;
	}
	ChildActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComp"));
	ChildActorComp->SetupAttachment(RootComp);
	ChildActorComp->SetChildActorClass(SparkEffectFactory);
	ChildActorComp->SetRelativeLocation(FVector(-80, 0, 0));
	ChildActorComp->SetRelativeRotation(FRotator(-90, 0, 0));
	ChildActorComp->SetRelativeScale3D(FVector(0.2, 0.2, 3));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_DragonArrow::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		ASB_Sova* MyPlayer = Cast<ASB_Sova>(GetOwner());
		Multicast_InitComplete(MyPlayer);
	}
}

// Called every frame
void ASB_DragonArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASB_DragonArrow::Multicast_InitComplete_Implementation(ASB_Sova* InPlayer)
{
	FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (InPlayer != nullptr)
	{
		AttachToComponent(InPlayer->arrowMesh, rules, TEXT("ArrowSocket"));
	}
}