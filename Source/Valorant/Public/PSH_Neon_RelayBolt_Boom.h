// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSH_Neon_RelayBolt_Boom.generated.h"

UCLASS()
class VALORANT_API APSH_Neon_RelayBolt_Boom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSH_Neon_RelayBolt_Boom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class USphereComponent* compShphere;
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	TSubclassOf<class ASH_RelayBolt_OverlapActor> reboltFat;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	float setSize =0;

	bool OnesNia = false;

// 	UPROPERTY(EditAnywhere,Category = "Skill_Q")
// 	class UNiagaraSystem* fireLoad;
	
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	UParticleSystem* ExParticle;
	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class UNiagaraSystem* Ring;


	
};
