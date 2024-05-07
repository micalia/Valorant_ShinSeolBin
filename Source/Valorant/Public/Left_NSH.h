// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Left_NSH.generated.h"

UCLASS()
class VALORANT_API ALeft_NSH : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeft_NSH();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;

};
