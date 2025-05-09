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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* ArrowHeadColl;

	UFUNCTION()
	void ArrowHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SMSovaArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* projectileComp;

	UPROPERTY(BlueprintReadWrite)
	float initSpeed = 500;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 maxBounceCount=2;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 currBounceCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayTime = 0.1;

	UFUNCTION(Server, Reliable)
	void ServerSpawnScanObj(AActor* ScanObjOwner);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AScanObj> ScanObjFactory;

	UPROPERTY(Replicated)
	bool bBounceEnd;
};
