// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Hook.generated.h"

UCLASS()
class VALORANT_API ASB_Hook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_Hook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing = OnRep_InitActor)
	bool bInitActor;

	UFUNCTION()
	void OnRep_InitActor();

	float DetectObjDistance = 150;

	bool bThrowHook = true;

	UPROPERTY(EditAnywhere)
	float HookSpeed = 7000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* HookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* HookColl;

	UFUNCTION()
	void HookHitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HitCheck();

// RPCº¸°£
	UPROPERTY(ReplicatedUsing = OnRep_ServerLocation)
	FVector ServerNewLocation;
	float ClientTimeSinceUpdate = 0.0f;
	float ClientTimeBetweenLastUpdate = 0.0f;

	UFUNCTION()
	void OnRep_ServerLocation();
};
