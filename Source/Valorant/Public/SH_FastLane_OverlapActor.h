// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_FastLane_OverlapActor.generated.h"

UCLASS()
class VALORANT_API ASH_FastLane_OverlapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASH_FastLane_OverlapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
UPROPERTY(EditAnywhere)
	class UBoxComponent * compBox;
UPROPERTY(EditAnywhere)
	class UStaticMeshComponent *compMesh;
UPROPERTY(EditAnywhere)
	float Scal = 0.1;
UPROPERTY(EditAnywhere, Category = DestroyTime)
	float Scal_Z = 2;
UPROPERTY(EditAnywhere,Category = DestroyTime)
	float CurTime = 0;
UPROPERTY(EditAnywhere)
	bool TimeOn = true;
UPROPERTY(EditAnywhere, Category = DestroyTime)
	bool ScalUp = true;
// 	UPROPERTY(EditAnywhere,Category = "Skill_C")
// 	class UNiagaraSystem* spake;
};
