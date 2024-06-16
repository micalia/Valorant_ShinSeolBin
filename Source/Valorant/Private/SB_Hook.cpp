// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_Hook.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Public/SB_Sova.h"
#include "Net/UnrealNetwork.h"
#include "../../Engine/Classes/Engine/EngineTypes.h"

// Sets default values
ASB_Hook::ASB_Hook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HookColl = CreateDefaultSubobject<USphereComponent>(TEXT("HookCollison"));
	HookColl->SetupAttachment(RootComponent);
	HookColl->SetSphereRadius(8);
	HookColl->SetCollisionProfileName(TEXT("Hook"));
	HookColl->SetEnableGravity(false);

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempHookMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/Hook/Hook.Hook'"));
	if (tempHookMesh.Succeeded()) {
		HookMesh->SetStaticMesh(tempHookMesh.Object);
	}
	HookMesh->SetupAttachment(HookColl);
	HookMesh->SetRelativeScale3D(FVector(6, 6, 4));
	HookMesh->SetRelativeLocation(FVector(-37, 0, 0));
	HookMesh->SetRelativeRotation(FRotator(-90, 0, 0));
	HookMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASB_Hook::BeginPlay()
{
	Super::BeginPlay();

	HookColl->OnComponentHit.AddDynamic(this, &ASB_Hook::HookHitCheck);

	bClientInitActor = true;
	if (HasAuthority()) {
		ASB_Sova* MyOwner = Cast<ASB_Sova>(GetOwner());
		if (MyOwner != nullptr) {
			MyOwner->AttachHook(this);
		}
	}
}

// Called every frame
void ASB_Hook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bThrowHook) {
		FVector startLoc = GetActorLocation();
		FVector endLoc = startLoc + GetActorForwardVector() * DetectObjDistance;
		FHitResult hitInfo;
		FCollisionQueryParams params;

		params.AddIgnoredActor(this);
		
		if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECollisionChannel::ECC_WorldDynamic, params))
		{
			HitCheck();
			SetActorLocation(hitInfo.ImpactPoint);
			return;
		}

		FVector HookP0 = HookColl->GetComponentLocation();
		FVector HookVT = GetActorForwardVector() * HookSpeed * DeltaTime;
		ServerNewLocation = HookP0 + HookVT;
		SetActorLocation(ServerNewLocation, true);
	}
	else {
		ClientTimeSinceUpdate += DeltaTime;
		if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER)
		{
			return;
		}
		const FVector EstimateLocation = ServerNewLocation + (GetActorForwardVector() * HookSpeed * ClientTimeBetweenLastUpdate);
		const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;

		FVector ClientLocation = GetActorLocation();
		const FVector ClientNewLocation = FMath::Lerp(ServerNewLocation, EstimateLocation, LerpRatio);
		SetActorLocation(ClientNewLocation);
	}
}

void ASB_Hook::OnRep_InitActor()
{
	ASB_Sova* MyOwner = Cast<ASB_Sova>(GetOwner());
	if (MyOwner != nullptr) {
		MyOwner->AttachHook(this);
	}
}


void ASB_Hook::HookHitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> HookHitCHK"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	HitCheck();
}

void ASB_Hook::HitCheck()
{
	if (HasAuthority()) {
		bThrowHook = false;
		HookColl->SetSimulatePhysics(false);
		if (ASB_Sova* MyOwner = Cast<ASB_Sova>(GetOwner())) {
			MyOwner->HookNotify();
		}
	}
}

void ASB_Hook::OnRep_ServerLocation()
{
	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;
}

void ASB_Hook::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_Hook, bClientInitActor);
	DOREPLIFETIME(ASB_Hook, ServerNewLocation);
}