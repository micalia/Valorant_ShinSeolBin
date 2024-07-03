// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_DragonArrow.generated.h"

UCLASS()
class VALORANT_API ASB_DragonArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_DragonArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SMSovaArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* ChildActorComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SparkEffectFactory;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitComplete(class ASB_Sova* InPlayer);

	UFUNCTION(Server, Reliable)
	void Server_DetachArrow();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachArrow();

	UFUNCTION(Server, Reliable)
	void Server_DragonArrowShot(FVector InDirVec);

public:
	UFUNCTION()
	void OnRep_LocAndRot();

	UPROPERTY(Replicated)
	bool bMove;
	UPROPERTY(BlueprintReadWrite)
	float InitSpeed = 2500;
	FVector InitDirVector;

	FVector Velocity;

	UPROPERTY(ReplicatedUsing = OnRep_LocAndRot)
	FVector P;
	UPROPERTY(ReplicatedUsing = OnRep_LocAndRot)
	FRotator NewRotation;

	FVector SpawnPos;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASB_TwoDragonArrowSpawn> DragonActorFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MiniDragonActorFactory;

	UPROPERTY(EditAnywhere)
	float DragonSpawnDist = 250;

	bool bSpawnDragon;

	UPROPERTY(EditAnywhere)
	float DragonActorSpawnDist = -200;
};
