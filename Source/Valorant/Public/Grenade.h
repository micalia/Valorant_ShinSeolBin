// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class VALORANT_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* GrenadeMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileComp;

	class ASB_Sova* SovaOwner;
	
	FVector MyVelocity;
	void Throw(FVector Velocity);

	UPROPERTY(Replicated)
	bool bHitCheck;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, 
				AActor* OtherActor, 
				UPrimitiveComponent* OtherComp, 
				FVector NormalImpulse, 
				const FHitResult& Hit);
	
	UPROPERTY()
	class UParticleSystem* ExplosionEffect;
	UFUNCTION(Server, Reliable)
	void ServerExplosion();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplosion();

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 480;
	UPROPERTY(EditAnywhere)
	int32 ExplosionDamage = 20;
};
