// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_RelayBolt_OverlapActor.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"

// Sets default values
ASH_RelayBolt_OverlapActor::ASH_RelayBolt_OverlapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	demageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
	RootComponent = demageBox;
	demageBox->SetBoxExtent(FVector(300));


}

// Called when the game starts or when spawned
void ASH_RelayBolt_OverlapActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(1.0f);

	demageBox->OnComponentBeginOverlap.AddDynamic(this, &ASH_RelayBolt_OverlapActor::OnComponentBeginOverlap);
}

// Called every frame
void ASH_RelayBolt_OverlapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASH_RelayBolt_OverlapActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ŽÆÀ¯Ç×¤¤;¤Ã¤¾¤·¤¤;Çë´Ï
	if (ABaseCharacter* hitPawn = Cast<ABaseCharacter>(OtherActor))
	{
		//UE_LOG(LogTemp,Warning,TEXT("boom"));

		demageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		hitPawn->Stun();
		//ServerHitProcess();

	}
}

