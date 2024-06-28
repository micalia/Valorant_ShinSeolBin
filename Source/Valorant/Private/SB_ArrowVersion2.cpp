// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_ArrowVersion2.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Public/ScanObj.h"
#include "SB_Sova.h"

// Sets default values
ASB_ArrowVersion2::ASB_ArrowVersion2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleCollComp"));
	SphereCollComp->SetupAttachment(RootComponent);
	SphereCollComp->SetSphereRadius(3.5);
	SphereCollComp->SetCollisionProfileName(TEXT("ScoutingArrow"));
	SphereCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SMSovaArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMSovaArrow"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempArrowMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/arrow/RegacyArrow/sovaArrow.sovaArrow'"));
	if (tempArrowMesh.Succeeded()) {
		SMSovaArrow->SetStaticMesh(tempArrowMesh.Object);
	}
	SMSovaArrow->SetupAttachment(SphereCollComp);
	SMSovaArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SMSovaArrow->SetRelativeScale3D(FVector(2));
	SMSovaArrow->SetRelativeLocation(FVector(0, 0, 141));
	SMSovaArrow->SetRelativeRotation(FRotator(-90, 0, 0));
	SMSovaArrow->SetCastShadow(false);

	static ConstructorHelpers::FClassFinder<AScanObj> tempScanObjFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_ScanObj.BP_ScanObj_C'"));
	if (tempScanObjFactory.Succeeded()) {
		ScanObjFactory = tempScanObjFactory.Class;
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_ArrowVersion2::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		ASB_Sova* MyPlayer = Cast<ASB_Sova>(GetOwner());
		Multicast_InitComplete(MyPlayer);
	}
}

// Called every frame
void ASB_ArrowVersion2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 움직임은 서버에서 처리
	if (HasAuthority() && bMove) {
		FVector startLoc = GetActorLocation();
		FVector endLoc = startLoc + GetActorForwardVector() * DetectObjDistance;
		FHitResult hitInfo;
		FCollisionQueryParams params;

		params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECollisionChannel::ECC_WorldDynamic, params))
		{
			ArrowReflection(hitInfo);
		}

// 등가속도 운동 공식 : v = v0 + at  >> 현재 속력에 중력 가속도를 더함
		Velocity = InitDirVector * InitSpeed;
		zVelocity += CustomGravity * DeltaTime;
		Velocity.Z += zVelocity;

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
	}
}

void ASB_ArrowVersion2::Multicast_InitComplete_Implementation(ASB_Sova* InPlayer)
{
	FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (InPlayer != nullptr)
	{
		AttachToComponent(InPlayer->arrowMesh, rules, TEXT("ArrowSocket"));
	}
}

void ASB_ArrowVersion2::OnRep_LocAndRot()
{
	SetActorLocation(P, true);
	SetActorRotation(NewRotation);
}

void ASB_ArrowVersion2::ServerSpawnScanObj_Implementation(class ASB_Sova* InOwnerSova)
{
	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnConfig.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnConfig.Owner = InOwnerSova;
	auto doFunc = [this](AActor* ObjectToModify)
		{
			AScanObj* ScanObjModify = Cast<AScanObj>(ObjectToModify);
			if (ScanObjModify)
			{
				ScanObjModify->ScanArrow = this;
			}
	};

	spawnConfig.CustomPreSpawnInitalization = doFunc;

	FMatrix ScanObjRotMatrix = FRotationMatrix::MakeFromX(LastImpactNormal);
	FRotator ScanObjRot = ScanObjRotMatrix.Rotator();

	AScanObj* ScanObj = GetWorld()->SpawnActor<AScanObj>(ScanObjFactory, GetActorLocation(), ScanObjRot, spawnConfig);
	MulticastHideArrowMesh();
}

void ASB_ArrowVersion2::MulticastHideArrowMesh_Implementation()
{
	SMSovaArrow->SetVisibility(false);
}

void ASB_ArrowVersion2::Server_DetachArrow_Implementation()
{
	ASB_Sova* MyPlayer = Cast<ASB_Sova>(GetOwner());
	Multicast_DetachArrow(MyPlayer);
}

void ASB_ArrowVersion2::Multicast_DetachArrow_Implementation(class ASB_Sova* InSova)
{
	FDetachmentTransformRules rules = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromActor(rules);
}

void ASB_ArrowVersion2::Server_ArrowShotInit_Implementation(float InInitSpeed, int32 BounceCount, FVector InDirVec)
{
	InitSpeed = InInitSpeed;
	maxBounceCount = BounceCount;
	InitDirVector = InDirVec;
	bMove = true;
}

void ASB_ArrowVersion2::ArrowReflection(FHitResult& InHitInfo)
{
	if (HasAuthority()) {  // 서버에서만 충돌처리
		SetActorLocation(InHitInfo.ImpactPoint);
		if (currBounceCount < maxBounceCount) {
			zVelocity = 0;
			currBounceCount++;
			// R(반사각) = L(입사각) + 2N(-L·N)
			FVector ShootDir = Velocity.GetSafeNormal();
			float Projection = FVector::DotProduct(-ShootDir, InHitInfo.ImpactNormal);
			FVector ReflectionVec = ShootDir + 2 * InHitInfo.ImpactNormal * Projection;
			InitDirVector = ReflectionVec;
		}// 발사시 설정된 충돌 횟수만큼 충돌하면, 화살 움직임 정지 후 적군 벽 투시 스캔시작
		else {
			LastImpactNormal = InHitInfo.ImpactNormal;
			bMove = false;
			SphereCollComp->SetSimulatePhysics(false);
			SphereCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (ASB_Sova* MySova = Cast<ASB_Sova>(GetOwner())) {
				ServerSpawnScanObj_Implementation(MySova);
			}
		}
	}
}

void ASB_ArrowVersion2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_ArrowVersion2, currBounceCount);
	DOREPLIFETIME(ASB_ArrowVersion2, bMove);
	DOREPLIFETIME(ASB_ArrowVersion2, P);
	DOREPLIFETIME(ASB_ArrowVersion2, NewRotation);
}