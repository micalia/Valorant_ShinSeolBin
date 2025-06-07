// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_ArrowVersion2.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
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
	// �������� �������� ó��
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

// ��ӵ� � ���� : v = v0 + at  >> ���� �ӷ¿� �߷� ���ӵ��� ����
		Velocity = InitDirVector * InitSpeed;
		zVelocity += CustomGravity * DeltaTime;
		Velocity.Z += zVelocity;

// ��� � ���� : P = P0 + vt 
		FVector P0 = GetActorLocation();
		FVector VT = Velocity * DeltaTime;
		P = P0 + VT;
		SetActorLocation(P, true);
// ���� ���͸� Ȱ���Ͽ� ȭ���� �����̴� �������� ȭ�� ȸ��(�ڿ������� ������ ����)
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
	Multicast_DetachArrow();
}

void ASB_ArrowVersion2::Multicast_DetachArrow_Implementation()
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
	if (HasAuthority()) {  // ���������� �浹ó��
		SetActorLocation(InHitInfo.ImpactPoint);
		if (currBounceCount < maxBounceCount) {
			zVelocity = 0;
			currBounceCount++;
			// R(�ݻ簢) = L(�Ի簢) + 2N(-L��N)
			FVector ShootDir = Velocity.GetSafeNormal();
			float Projection = FVector::DotProduct(-ShootDir, InHitInfo.ImpactNormal);
			FVector ReflectionVec = ShootDir + 2 * InHitInfo.ImpactNormal * Projection;
			InitDirVector = ReflectionVec;
		}// �߻�� ������ �浹 Ƚ����ŭ �浹�ϸ�, ȭ�� ������ ���� �� ���� �� ���� ��ĵ����
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