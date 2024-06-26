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
	SphereCollComp->SetupAttachment(CollTransformComp);
	SphereCollComp->SetSphereRadius(3.5);
	SphereCollComp->SetCollisionProfileName(TEXT("ScoutingArrow"));

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

	SphereCollComp->OnComponentHit.AddDynamic(this, &ASB_ArrowVersion2::ArrowHeadHit);

	if (HasAuthority()) {
		ASB_Sova* MyPlayer = Cast<ASB_Sova>(GetOwner());
		Multicast_InitComplete(MyPlayer);
	}
	/*if (HasAuthority()) {
		if (APlayerController* Pc = Cast<APlayerController>(GetOwner())) {
			if (ASB_Sova* Sova = Cast<ASB_Sova>(Pc->GetPawn())) {
				AttachToComponent(Sova->arrowMesh, AttachRule, TEXT("ArrowSocket"));
			}
		}
	}
	else {
	if(APlayerController* Pc = Cast<APlayerController>(GetOwner())){
		if (ASB_Sova* Sova = Cast<ASB_Sova>(Pc->GetPawn())) {
			bool bResult = AttachToComponent(Sova->arrowMesh, AttachRule, TEXT("ArrowSocket"));
			if (bResult) {
				GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Succ"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> fail"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
			}
		}
	}*/
	/*if (!HasAuthority()) {
		if (APlayerController* Pc = Cast<APlayerController>(GetOwner())) {
			if (ASB_Sova* Sova = Cast<ASB_Sova>(Pc->GetPawn())) {
				bool bResult = AttachToComponent(Sova->arrowMesh, AttachRule, TEXT("ArrowSocket"));
				if (bResult) {
					GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Succ"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> fail"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
				}
			}
		}

	}*/
	/*if (HasAuthority()) {
		FAttachmentTransformRules AttachRule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
		if (APlayerController* Pc = Cast<APlayerController>(GetOwner())) {
			if (ASB_Sova* Sova = Cast<ASB_Sova>(Pc->GetPawn())) {
				bool bResult = AttachToComponent(Sova->arrowMesh, AttachRule, TEXT("ArrowSocket"));
				if (bResult) {
					GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Succ"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> fail"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
				}
			}
		}

	}*/
}


// Called every frame
void ASB_ArrowVersion2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// �������� �������� ó��
	if (HasAuthority() && bMove) {
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
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Init Multicast"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (InPlayer != nullptr)
	{
		bool bAttachResult = AttachToComponent(InPlayer->arrowMesh, rules, TEXT("ArrowSocket"));
		if (bAttachResult) {
			GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> attach SU"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> attach Fai"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
		}
	}
}

void ASB_ArrowVersion2::OnRep_LocAndRot()
{
	SetActorLocation(P, true);
	SetActorRotation(NewRotation);
}
// SphereCollComp->OnComponentHit.AddDynamic(this, &ASB_ArrowVersion2::ArrowHeadHit);
void ASB_ArrowVersion2::ArrowHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{ // ���� �浹�� �ݻ簢 ���
	auto name =  Hit.GetActor()->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Arrow hit name : %s"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S")), *name), true, FVector2D(1.5f, 1.5f));
	if (HasAuthority()) {  // ���������� �浹ó��
		if (currBounceCount < maxBounceCount) {
			zVelocity = 0;
			currBounceCount++;
			// R(�ݻ簢) = L(�Ի簢) + 2N(-L��N)
			FVector ShootDir = Velocity.GetSafeNormal();
			float Projection = FVector::DotProduct(-ShootDir, Hit.ImpactNormal);
			FVector ReflectionVec = ShootDir + 2 * Hit.ImpactNormal * Projection;
			InitDirVector = ReflectionVec;
		}// �߻�� ������ �浹 Ƚ����ŭ �浹�ϸ�, ȭ�� ������ ���� �� ���� �� ���� ��ĵ����
		else {
			LastImpactNormal = Hit.ImpactNormal;
			bMove = false;
			SphereCollComp->SetSimulatePhysics(false);
			SphereCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ServerSpawnScanObj_Implementation(GetOwner());
		}
	}
}

void ASB_ArrowVersion2::ServerSpawnScanObj_Implementation(AActor* ScanObjOwner)
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

void ASB_ArrowVersion2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_ArrowVersion2, currBounceCount);
	DOREPLIFETIME(ASB_ArrowVersion2, bMove);
	DOREPLIFETIME(ASB_ArrowVersion2, P);
	DOREPLIFETIME(ASB_ArrowVersion2, NewRotation);
}