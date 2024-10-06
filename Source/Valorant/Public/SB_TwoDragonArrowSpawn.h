// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SB_TwoDragonArrowSpawn.generated.h"

USTRUCT()
struct FPlayerDamage {
	GENERATED_BODY()

	UPROPERTY()
	class ABaseCharacter* DamagedPlayer = nullptr;
	UPROPERTY()
	bool bCanAttack = false;

	UPROPERTY()
	FTimerHandle DamageTermHandle;

	bool operator==(const FPlayerDamage& InPlayerDamage) const {
		return DamagedPlayer == InPlayerDamage.DamagedPlayer;
	}

	friend FORCEINLINE uint32 GetTypeHash(const FPlayerDamage& InPlayerDamageData) {
		return GetTypeHash(InPlayerDamageData.DamagedPlayer);
	}
};

UCLASS()
class VALORANT_API ASB_TwoDragonArrowSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASB_TwoDragonArrowSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* AttackColl;

	ABaseCharacter* ThisOwner;

	TSet<FPlayerDamage> DamagedPlayers;

	UFUNCTION()
	void OnAttackOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
					AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, 
					int32 OtherBodyIndex);
};
