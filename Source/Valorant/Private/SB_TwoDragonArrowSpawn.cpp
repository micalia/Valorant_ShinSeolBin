// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_TwoDragonArrowSpawn.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASB_TwoDragonArrowSpawn::ASB_TwoDragonArrowSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	AttackColl = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackColl"));
	AttackColl->SetupAttachment(RootComponent);
	AttackColl->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackColl->SetCapsuleRadius(384);
	AttackColl->SetCapsuleHalfHeight(830);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_TwoDragonArrowSpawn::BeginPlay()
{
	Super::BeginPlay();

	ThisOwner = Cast<ABaseCharacter>(GetOwner());
	AttackColl->OnComponentBeginOverlap.AddDynamic(this, &ASB_TwoDragonArrowSpawn::OnAttackOverlap);
	AttackColl->OnComponentEndOverlap.AddDynamic(this, &ASB_TwoDragonArrowSpawn::OnEndOverlap);
}

// Called every frame
void ASB_TwoDragonArrowSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ThisOwner != nullptr && HasAuthority()){
		for (auto& Element : DamagedPlayers)
		{
			if(Element.bCanAttack == false){
				GetWorld()->GetTimerManager().ClearTimer(Element.DamageTermHandle);
				Element.DamagedPlayer->ServerDamagedHealth_Implementation(20, ThisOwner, true);
				Element.bCanAttack = true;
				GetWorld()->GetTimerManager().SetTimer(Element.DamageTermHandle, FTimerDelegate::CreateLambda([&]() {
					Element.bCanAttack = false;
				}), 0.5, false);
			}
		}
	}
}

void ASB_TwoDragonArrowSpawn::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (ABaseCharacter* DamagedPlayer = Cast<ABaseCharacter>(OtherActor)) {
		auto MyOwner = Cast<ABaseCharacter>(GetOwner());
		if(MyOwner == DamagedPlayer) return;
			FPlayerDamage DamagedPlayerInfo;
			DamagedPlayerInfo.DamagedPlayer = DamagedPlayer;
			if(DamagedPlayers.Find(DamagedPlayerInfo) == nullptr){
				FTimerHandle TermHandle;
				DamagedPlayerInfo.DamagedPlayer = DamagedPlayer;
				DamagedPlayerInfo.bCanAttack = false;
				DamagedPlayerInfo.DamageTermHandle = TermHandle;
				DamagedPlayers.Add(DamagedPlayerInfo);
			}
		}
	}
}

void ASB_TwoDragonArrowSpawn::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority()) {
		if (ABaseCharacter* DamagedPlayer = Cast<ABaseCharacter>(OtherActor)) {
			auto MyOwner = Cast<ABaseCharacter>(GetOwner());
			if (MyOwner == DamagedPlayer) return;
			FPlayerDamage DamagedPlayerInfo;
			DamagedPlayerInfo.DamagedPlayer = DamagedPlayer;

			if (DamagedPlayers.Find(DamagedPlayerInfo) != nullptr) {
				auto RemResult = DamagedPlayers.Remove(DamagedPlayerInfo);
			}
		}
	}
}

