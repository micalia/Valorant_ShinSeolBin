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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* SMSovaArrow;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitComplete(class ASB_Sova* InPlayer);

	UPROPERTY(BlueprintReadWrite)
	float InitSpeed = 2000;
	FVector InitDirVector;

	FVector Velocity;
	
	UPROPERTY(ReplicatedUsing = OnRep_LocAndRot)
	FVector P;
	UPROPERTY(ReplicatedUsing = OnRep_LocAndRot)
	FRotator NewRotation;

	UFUNCTION()
	void OnRep_LocAndRot();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AScanObj> ScanObjFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 maxBounceCount=2;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 currBounceCount;

	UPROPERTY(EditAnywhere)
	float CustomGravity = -450.0;
	float zVelocity = 0;
	
	UPROPERTY(Replicated)
	bool bMove;

	UFUNCTION(Server, Reliable)
	void ServerSpawnScanObj(class ASB_Sova* InOwnerSova);

	FVector LastImpactNormal;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHideArrowMesh();

	UFUNCTION(Server, Reliable)
	void Server_DetachArrow();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachArrow(class ASB_Sova* InSova);

	UFUNCTION(Server, Reliable)
	void Server_ArrowShotInit(float InInitSpeed, int32 BounceCount, FVector InDirVec);

	void ArrowReflection(FHitResult& InHitInfo);
	UPROPERTY(EditAnywhere)
	float DetectObjDistance = 200;
};
