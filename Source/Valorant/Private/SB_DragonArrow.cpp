// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_DragonArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "SB_Sova.h"
#include "../../Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "SB_TwoDragonArrowSpawn.h"

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

	ChildActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComp"));
	ChildActorComp->SetupAttachment(RootComp);
	//ChildActorComp->SetChildActorClass(SparkEffectFactory);
	ChildActorComp->SetRelativeLocation(FVector(-80, 0, 0));
	ChildActorComp->SetRelativeRotation(FRotator(-90, 0, 0));
	ChildActorComp->SetRelativeScale3D(FVector(0.2, 0.2, 3));

	static ConstructorHelpers::FClassFinder<ASB_TwoDragonArrowSpawn> tempDragonActorFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/DragonArrow/BP_2DragonArrowSpawn.BP_2DragonArrowSpawn_C'"));
	if (tempDragonActorFactory.Succeeded()) {
		DragonActorFactory = tempDragonActorFactory.Class;
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_DragonArrow::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		ASB_Sova* MyPlayer = Cast<ASB_Sova>(GetOwner());
		Multicast_InitComplete(MyPlayer);
		SpawnPos = GetActorLocation();
	}
}

// Called every frame
void ASB_DragonArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 움직임은 서버에서 처리
	if (HasAuthority() && bMove) {
		Velocity = InitDirVector * InitSpeed;

		// 등속 운동 공식 : P = P0 + vt 
		FVector P0 = GetActorLocation();
		FVector VT = Velocity * DeltaTime;
		P = P0 + VT;
		SetActorLocation(P, true);
		// 방향 벡터를 활용하여 화살이 움직이는 방향으로 화살 회전(자연스러운 움직임 구현)
		FVector RotDirVec = (P - P0).GetSafeNormal();
		FMatrix RotationMatrix = FRotationMatrix::MakeFromX(RotDirVec);
		NewRotation = RotationMatrix.Rotator();
		SetActorRotation(NewRotation);

		FVector ShotDist = SpawnPos - GetActorLocation();
		if (ShotDist.Length() > DragonSpawnDist) {
			if(bSpawnDragon == false) {
				bSpawnDragon = true;
				FVector SpawnP0 = GetActorLocation();
				FVector SpawnP = SpawnP0 + GetActorForwardVector() * DragonActorSpawnDist;

				FActorSpawnParameters SpawnConfig;
				SpawnConfig.Owner = GetOwner();
				AActor* Dragon = GetWorld()->SpawnActor<AActor>(DragonActorFactory, SpawnP, GetActorRotation(), SpawnConfig);
			}
		}
	}
}

void ASB_DragonArrow::Multicast_InitComplete_Implementation(ASB_Sova* InPlayer)
{
	FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (InPlayer != nullptr)
	{
		AttachToComponent(InPlayer->arrowMesh, rules, TEXT("ArrowSocket"));
	}
}

void ASB_DragonArrow::Server_DetachArrow_Implementation()
{
	Multicast_DetachArrow();
}

void ASB_DragonArrow::Multicast_DetachArrow_Implementation()
{
	FDetachmentTransformRules rules = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromActor(rules);
}

void ASB_DragonArrow::Server_DragonArrowShot_Implementation(FVector InDirVec)
{
	InitDirVector = InDirVec;
	bMove = true;
}

void ASB_DragonArrow::OnRep_LocAndRot()
{
	SetActorLocation(P, true);
	SetActorRotation(NewRotation);
}

void ASB_DragonArrow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_DragonArrow, bMove);
	DOREPLIFETIME(ASB_DragonArrow, P);
	DOREPLIFETIME(ASB_DragonArrow, NewRotation);
}