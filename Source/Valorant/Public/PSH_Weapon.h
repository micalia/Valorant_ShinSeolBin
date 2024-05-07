// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PSH_Weapon.generated.h"

UCLASS()
class VALORANT_API APSH_Weapon : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	APSH_Weapon();


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, Category=MySettings)
	class UBoxComponent* boxComp;

	/** ¾×ÅÍÀÇ ½ºÄÌ·¹Åæ ¸Å½Ã*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* meshComp;

	UFUNCTION()
	void ReleaseWeapon(class ASH_Neon* player);

	void Reloading(class ASH_Neon* player);
	
	void Fire(FHitResult& FireHit, const FVector& Start, const FVector& End);
	// ÆÄÆ¼Å¬ 
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	class	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)

	class USoundCue* FireSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GrabWeapon(class ASH_Neon* player);

	UPROPERTY()
	int32 ammo = 30;
	int32 attackPower = 20;
	int32 magazine = 3;
	float fireInterval = 0.1f;
	// ÅºÃ¢
};
