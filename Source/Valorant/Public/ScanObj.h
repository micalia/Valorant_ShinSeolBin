// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScanObj.generated.h"

UCLASS()
class VALORANT_API AScanObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScanObj();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* MPCScanPos;
	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* MPCScanObjRadius;

	UPROPERTY(EditAnywhere)
	float ScanTime = 3;
	float ScanCurrTime;

	int32 WaveCount = 3;
	int32 CurrWaveCount;

	class ASB_Sova* MyPlayer;
	class ASB_Sova* OwnerPlayer;
	class ASB_ArrowVersion2* ScanArrow;

	UFUNCTION(Server, Reliable)
	void ServerHideScanEffect();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHideScanEffect();

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ScanObjMeshComp;
};
