// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSH_Neon_RelayBolt.generated.h"

UCLASS()
class VALORANT_API APSH_Neon_RelayBolt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSH_Neon_RelayBolt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	FVector Dir;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	float power = 5000.0f;
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	int32 destroyNum =0;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class USphereComponent* compShphere;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	TSubclassOf<class APSH_Neon_RelayBolt_Boom> boomFat;

	UFUNCTION(Server,Unreliable)
	void SeverShotBolt(class ASH_Neon* neon);


	UPROPERTY(EditAnywhere, Category = "Skill_Q")
		class UProjectileMovementComponent* ProjectileComp;

	UFUNCTION()
	void OnComponentHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
