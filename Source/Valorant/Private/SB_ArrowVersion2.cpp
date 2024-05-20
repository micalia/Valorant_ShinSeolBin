// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_ArrowVersion2.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Public/ScanObj.h"

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
}

// Called every frame
void ASB_ArrowVersion2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bMove) {
		// v = v0 + at  >> 현재 속력에 중력 가속도를 더함
		Velocity = InitDirVector * InitSpeed;
		zVelocity += CustomGravity * DeltaTime;
		Velocity.Z += zVelocity;

		// P = P0 + vt
		FVector P0 = GetActorLocation();
		FVector VT = Velocity * DeltaTime;
		FVector P = P0 + VT;
		SetActorLocation(P, true);
	
		FVector RotDirVec = (P - P0).GetSafeNormal();
		FMatrix RotationMatrix = FRotationMatrix::MakeFromX(RotDirVec);
		FRotator NewRotation = RotationMatrix.Rotator();
		SetActorRotation(NewRotation);
	}
}

void ASB_ArrowVersion2::ArrowHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (currBounceCount < maxBounceCount) {
		zVelocity = 0;
		currBounceCount++;
		GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> CurrCount : %d"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S")), currBounceCount), true, FVector2D(1.5f, 1.5f));
		// R(반사각) = L(입사각) + 2N(-L·N)
		FVector ShootDir = Velocity.GetSafeNormal();
		float Projection = FVector::DotProduct(-ShootDir, Hit.ImpactNormal);
		FVector ReflectionVec = ShootDir + 2 * Hit.ImpactNormal * Projection;

		InitDirVector = ReflectionVec;
	}
	else {
		bMove = false;
		SphereCollComp->SetSimulatePhysics(false);
		SphereCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASB_ArrowVersion2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_ArrowVersion2, currBounceCount);
}