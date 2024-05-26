// Fill out your copyright notice in the Description page of Project Settings.

#include "SB_Arrow.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ScanObj.h"

// Sets default values
ASB_Arrow::ASB_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileComp"));
	ArrowHeadColl = CreateDefaultSubobject<USphereComponent>(TEXT("ArrowHeadColl"));
	SetRootComponent(ArrowHeadColl);

	/*static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> tempPMBounceArrow(TEXT("/Script/PhysicsCore.PhysicalMaterial'/Game/SB/Materials/BallDropPhys.BallDropPhys'"));
	if (tempPMBounceArrow.Succeeded()) {
		ArrowHeadColl->SetPhysMaterialOverride(tempPMBounceArrow.Object);
	}*/

	ArrowHeadColl->SetCollisionProfileName(TEXT("ScoutingArrow"));
	ArrowHeadColl->SetSphereRadius(55);
	ArrowHeadColl->SetSimulatePhysics(true);
	ArrowHeadColl->SetNotifyRigidBodyCollision(true);
	ArrowHeadColl->BodyInstance.bLockXRotation = true;
	ArrowHeadColl->BodyInstance.bLockYRotation = true;
	ArrowHeadColl->BodyInstance.bLockZRotation = true;
	ArrowHeadColl->SetIsReplicated(true);

	SMSovaArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMSovaArrow"));
	SMSovaArrow->SetupAttachment(RootComponent);
	SMSovaArrow->SetRelativeScale3D(FVector(2));
	SMSovaArrow->SetRelativeLocation(FVector(-111, 0, 0));
	SMSovaArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SMSovaArrow->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempArrowMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/arrow/RegacyArrow/sovaArrow.sovaArrow'"));
	if (tempArrowMesh.Succeeded()) {
		SMSovaArrow->SetStaticMesh(tempArrowMesh.Object);
	}

	bReplicates = true;
	SetReplicateMovement(true);
	projectileComp->SetIsReplicated(true);

	static ConstructorHelpers::FClassFinder<AScanObj> tempScanObjFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_ScanObj.BP_ScanObj_C'"));
	if (tempScanObjFactory.Succeeded()) {
		ScanObjFactory = tempScanObjFactory.Class;
	}
	
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
	
}

// Called when the game starts or when spawned
void ASB_Arrow::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents : %f"), projectileComp->InitialSpeed)
	projectileComp->SetUpdatedComponent(ArrowHeadColl);
	ArrowHeadColl->OnComponentHit.AddDynamic(this, &ASB_Arrow::ArrowHeadHit);
}

// Called every frame
void ASB_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		if (!bBounceEnd) {
			FRotator Rot = UKismetMathLibrary::MakeRotationFromAxes(GetVelocity().GetSafeNormal(), FVector(0), FVector(0));
			SMSovaArrow->SetWorldRotation(Rot);
		}
		if (bHit) {
			currTime+=DeltaTime;
			if (currTime > DelayTime) {
				bHit = false;
			}
		}
		else {
			currTime = 0;
		}
	}
}

void ASB_Arrow::ArrowHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) {
		if (!bHit) {
			bHit = true;
			currBounceCount++;
			FString HitName = OtherActor->GetName();
			FString HitComp = OtherComp->GetName();

			if (currBounceCount > maxBounceCount) {
				bBounceEnd = true;
				projectileComp->ProjectileGravityScale = 0;
				ArrowHeadColl->SetEnableGravity(false);
				ArrowHeadColl->SetSimulatePhysics(false);
				ServerSpawnScanObj_Implementation(GetOwner());
			}
		}
	}
}

void ASB_Arrow::ServerSpawnScanObj_Implementation(AActor* ScanObjOwner)
{
	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnConfig.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnConfig.Owner = ScanObjOwner;
	auto doFunc = [this](AActor* ObjectToModify)
		{
			AScanObj* ScanObjModify = Cast<AScanObj>(ObjectToModify);
			if (ScanObjModify)
			{
				//ScanObjModify->ScanArrow = this;
		}
	};

	spawnConfig.CustomPreSpawnInitalization = doFunc;

	AScanObj* ScanObj = GetWorld()->SpawnActor<AScanObj>(ScanObjFactory, GetActorTransform(), spawnConfig);
}

void ASB_Arrow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_Arrow, currBounceCount);
	DOREPLIFETIME(ASB_Arrow, bBounceEnd);
}