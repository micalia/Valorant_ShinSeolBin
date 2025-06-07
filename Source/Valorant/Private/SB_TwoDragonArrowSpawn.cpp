// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_TwoDragonArrowSpawn.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

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

	static ConstructorHelpers::FClassFinder<AActor> tempSpawnDragonArrowEffectFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/DragonArrow/BP_SpawnDragonArrowEffect.BP_SpawnDragonArrowEffect_C'"));
	if (tempSpawnDragonArrowEffectFactory.Succeeded()) {
		SpawnDragonArrowEffectFactory = tempSpawnDragonArrowEffectFactory.Class;
	}

	DragonComp = CreateDefaultSubobject<USphereComponent>(TEXT("DragonComp"));
	DragonComp->SetupAttachment(RootComp);
	DragonComp->SetRelativeRotation(FRotator(0, 0, 60));
	DragonComp->SetSphereRadius(32);
	DragonComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DragonBoxColl1 = CreateDefaultSubobject<UBoxComponent>(TEXT("DragonBoxColl1"));
	DragonBoxColl1->SetupAttachment(DragonComp);
	DragonBoxColl1->SetRelativeRotation(FRotator(0, 0, -90));
	DragonBoxColl1->SetBoxExtent(FVector(32));
	DragonComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DragonBoxColl2 = CreateDefaultSubobject<UBoxComponent>(TEXT("DragonBoxColl2"));
	DragonBoxColl2->SetRelativeRotation(FRotator(0, 0, 90));
	DragonBoxColl2->SetupAttachment(DragonComp);
	DragonBoxColl2->SetBoxExtent(FVector(32));
	DragonComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FClassFinder<AActor> tempDragonMeshActorFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/DragonArrow/BP_DragonMeshActor.BP_DragonMeshActor_C'"));
	if (tempDragonMeshActorFactory.Succeeded()) {
		DragonMeshActorFactory = tempDragonMeshActorFactory.Class;
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_TwoDragonArrowSpawn::BeginPlay()
{
	Super::BeginPlay();

	ThisOwner = Cast<ABaseCharacter>(GetOwner());
	AttackColl->OnComponentBeginOverlap.AddDynamic(this, &ASB_TwoDragonArrowSpawn::OnAttackOverlap);
	AttackColl->OnComponentEndOverlap.AddDynamic(this, &ASB_TwoDragonArrowSpawn::OnEndOverlap);

	if (HasAuthority()) {
		FVector SpawnLoc = GetActorForwardVector() * DragonSpawnDist + GetActorLocation();
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.Owner = GetOwner();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLoc);
		SpawnTransform.SetScale3D(FVector(4));
		GetWorld()->SpawnActor<AActor>(SpawnDragonArrowEffectFactory, SpawnTransform, SpawnConfig);
		AttackColl->SetWorldLocation(SpawnLoc);

		FTransform DragonSpawnTransform1 = DragonBoxColl1->GetComponentTransform();
		DragonSpawnTransform1.SetScale3D(FVector(EffectScale));
		Dragon1 = GetWorld()->SpawnActor<AActor>(DragonMeshActorFactory, DragonSpawnTransform1, SpawnConfig);

		FTransform DragonSpawnTransform2 = DragonBoxColl2->GetComponentTransform();
		DragonSpawnTransform2.SetScale3D(FVector(EffectScale));
		Dragon2 = GetWorld()->SpawnActor<AActor>(DragonMeshActorFactory, DragonSpawnTransform2, SpawnConfig);

		SetLifeSpan(DelayDestroyTime);
	}
}

// Called every frame
void ASB_TwoDragonArrowSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ThisOwner != nullptr && HasAuthority()){ // 서버에서 데미지 처리
		for (auto& Element : DamagedPlayers) // 콜리전과 충돌된 플레이어들 순회
		{
			if(Element.bCanAttack == false){ // 플레이어에게 데미지를 줄 수 있는 상태인가?
				GetWorld()->GetTimerManager().ClearTimer(Element.DamageTermHandle);
				Element.DamagedPlayer->ServerDamagedHealth_Implementation(Power, ThisOwner, true);
				Element.bCanAttack = true; // 플레이어가 용의 콜리전과 충돌하고 있더라도 0.5초에 한번씩 데미지를 입힘 
				GetWorld()->GetTimerManager().SetTimer(Element.DamageTermHandle, FTimerDelegate::CreateLambda([&]() {
					Element.bCanAttack = false;
				}), 0.5, false);
			}
		}
		#pragma region AttackCollisionMove
		FVector Dragon1BoneLoc = Dragon1->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(TEXT("Bone011"));
		FVector Dragon2BoneLoc = Dragon2->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(TEXT("Bone011"));
		FVector NewDragonCenterLoc = ((Dragon1BoneLoc + Dragon2BoneLoc) / 2 ) + (GetActorForwardVector() * Velocity) * DeltaTime;
		AttackColl->SetWorldLocation(NewDragonCenterLoc);
		#pragma endregion
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

