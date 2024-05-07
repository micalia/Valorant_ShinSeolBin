// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SH_HitTestPlayer.generated.h"

UCLASS()
class VALORANT_API ASH_HitTestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASH_HitTestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void HitProcess();

	UPROPERTY(EditAnywhere, DisplayName="Max HP", Category = HP)
	int32 maxHealth = 100;

	UPROPERTY(EditAnywhere, Category = HP)
	class UAnimMontage* hitMontage;
	UPROPERTY(EditAnywhere, Category = HP)
	class UParticleSystem* hitParticle;

	void DamagedHealth(int32 value);

	void DieProcess();

private:
	UPROPERTY()
	int32 health = 0;
};
