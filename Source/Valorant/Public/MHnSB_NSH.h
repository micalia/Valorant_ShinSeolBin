// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MHnSB_NSH.generated.h"

UCLASS()
class VALORANT_API AMHnSB_NSH : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMHnSB_NSH();

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
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//class UNiagaraComponent* smoke;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=MHSettings)
	float Speed=800;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= MHSettings)
	FVector TargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	bool IsStop = false;

	/*UFUNCTION(Server, Reliable)
	void ServerToButtomMH();

	UFUNCTION(BlueprintImplementableEvent)
	void ToButtomMH_Implementation();*/
	UFUNCTION(BlueprintImplementableEvent)
	void ToButtomMH();
};
