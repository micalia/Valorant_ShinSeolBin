// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class VALORANT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	ABaseCharacter* playerInstance;

	UPROPERTY()
	class UPlayerFireComponent* fireComp;

	UPROPERTY(VisibleAnywhere, Category=MySettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* MuzzleFlashComp;

	// ÆÄÆ¼Å¬
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Anim)
	class UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category = Anim)
	class UAnimMontage* FireMontage;

private:

	UFUNCTION()
	void GrabWeapon(class ABaseCharacter* player);
		int32 ammo = 5;
		int32 attackPower = 5;
		int32 magazine = 3;
		float fireInterval = 0.3f;

};
