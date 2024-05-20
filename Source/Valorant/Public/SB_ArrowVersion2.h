// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_ArrowVersion2.generated.h"

UCLASS()
class VALORANT_API ASB_ArrowVersion2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_ArrowVersion2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* ArrowHeadComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* CollTransformComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SMSovaArrow;

	UPROPERTY(BlueprintReadWrite)
	float InitSpeed = 2000;
	FVector InitDirVector;

	FVector Velocity;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AScanObj> ScanObjFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 maxBounceCount=2;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 currBounceCount;

	UPROPERTY(EditAnywhere)
	float CustomGravity = -450.0;
	float zVelocity = 0;
	
	bool bMove = true;

	UFUNCTION()
	void ArrowHeadHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
