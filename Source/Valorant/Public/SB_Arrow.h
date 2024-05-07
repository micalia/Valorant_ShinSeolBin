// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_Arrow.generated.h"

UCLASS()
class VALORANT_API ASB_Arrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_Arrow();

protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnScanObj(FTransform transform);
	UFUNCTION(NetMultiCast, Reliable)
	void Server_SpawnScanObj(FTransform transform);*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* projectileComp;

	UPROPERTY(BlueprintReadWrite)
	float initSpeed = 500;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 maxBounceCount=2;
	UPROPERTY(BlueprintReadWrite)
	int32 currBounceCount;


};
