// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanObj.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Net/UnrealNetwork.h"
//#include "../Public/SB_Arrow.h"
#include "../Public/SB_ArrowVersion2.h"

// Sets default values
AScanObj::AScanObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> tempMPCScanPos(TEXT("/Script/Engine.MaterialParameterCollection'/Game/SB/Materials/PC_ScanPos.PC_ScanPos'"));
	if (tempMPCScanPos.Succeeded()) {
		MPCScanPos = tempMPCScanPos.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> tempMPCScanObjRadius(TEXT("/Script/Engine.MaterialParameterCollection'/Game/SB/Materials/PC_Radius.PC_Radius'"));
	if (tempMPCScanObjRadius.Succeeded()) {
		MPCScanObjRadius = tempMPCScanObjRadius.Object;
	}

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	ScanObjMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScanObjMeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempScanObjMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/ArrowScanObj/ArrowScanObj.ArrowScanObj'"));
	if (tempScanObjMesh.Succeeded()) {
		ScanObjMeshComp->SetStaticMesh(tempScanObjMesh.Object);
	}
	ScanObjMeshComp->SetupAttachment(RootComp);
	ScanObjMeshComp->SetRelativeScale3D(FVector(2.4));
	ScanObjMeshComp->SetRelativeRotation(FRotator(270, 0, 0));
	ScanObjMeshComp->SetRelativeLocation(FVector(-5, 0, 0));
	ScanObjMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AScanObj::BeginPlay()
{
	Super::BeginPlay();
	
	MyPlayer = GetWorld()->GetFirstPlayerController();
	auto CurrPos = GetActorLocation();
	FLinearColor SpawnPos = FLinearColor(CurrPos.X, CurrPos.Y, CurrPos.Z, 1);

	if (MyPlayer == GetOwner()) {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPCScanPos, TEXT("ScanPos"), SpawnPos);
	}
	else {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPCScanPos, TEXT("EnemyScanPos"), SpawnPos);
	}
}

// Called every frame
void AScanObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrWaveCount < WaveCount) {
		ScanCurrTime+=DeltaTime;
		if (ScanCurrTime < ScanTime) {
			float alpha = ScanCurrTime / ScanTime;
			alpha *= 7;
			if (MyPlayer == GetOwner()) {
				UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCScanObjRadius, TEXT("Radius"), alpha);
 			}
			else {
				UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCScanObjRadius, TEXT("EnemyRadius"), alpha);
			}
		}
		else {
			CurrWaveCount++;
			ScanCurrTime = 0;
		}
	}
	else {
		if (HasAuthority()) {
			ServerHideScanEffect_Implementation();
		}
		else {
			ServerHideScanEffect();
		}
	}
}

void AScanObj::ServerHideScanEffect_Implementation()
{
	MulticastHideScanEffect();
}

void AScanObj::MulticastHideScanEffect_Implementation()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCScanObjRadius, TEXT("Radius"), 0);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCScanObjRadius, TEXT("EnemyRadius"), 0);
	if (HasAuthority()) {
		if (ScanArrow) { 
			ScanArrow->Destroy();
		}
		Destroy(1);
	}
}
