// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_RelayBolt_OverlapActor.generated.h"

UCLASS()
class VALORANT_API ASH_RelayBolt_OverlapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASH_RelayBolt_OverlapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	class UBoxComponent* demageBox;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	UParticleSystem* ExParticle;

	UFUNCTION()
	void OnComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
