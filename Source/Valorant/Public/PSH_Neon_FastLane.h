// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSH_Neon_FastLane.generated.h"

UCLASS()
class VALORANT_API APSH_Neon_FastLane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSH_Neon_FastLane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector Start;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector Dir;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector endLoc;
	
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector leftEndLoc;

	UPROPERTY(EditAnywhere,Category = "Skill_C")
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	class USphereComponent* compShphere;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	TSubclassOf<class ASH_FastLane_OverlapActor> fastLane_OverlapActorFat;


	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector rightEndLoc;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	float power = 10000.0f;
	bool isShot = false;

	UPROPERTY(EditAnywhere,Category = "Skill_C")
	float	DistanceToMove = 4500;
	float TotalDistanceMoved = 0;



	UPROPERTY(EditAnywhere,Category = "Skill_C")
	class UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	class UParticleSystem * loadParticle;

	UPROPERTY(EditAnywhere,Category = "Skill_C")
	class UNiagaraSystem* spake;

	void AdjustParticleLength(const FVector& StartLocation, const FVector& EndLocation);

	void TestLine(FVector& StartLocation, FVector& EndLocation);

	void SpawnWall( FVector& StartLocation);

	UFUNCTION()
	void OnComponentHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
