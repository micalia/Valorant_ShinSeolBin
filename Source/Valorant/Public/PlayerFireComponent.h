// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFireComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedDel, int32 /* AmmoCnt */);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UPlayerFireComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:

	UPlayerFireComponent();
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	void Reload();
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Reload(int32 BulletCount);
	FORCEINLINE void SetOwningWeapon(class ABaseWeapon* weapon) { owningWeapon = weapon; }
	FORCEINLINE class ABaseWeapon* GetOwningWeapon() const { return owningWeapon; }
	FORCEINLINE int32 GetAmmo() const { return ammo; }
	
	FORCEINLINE void SetAmmo(int32 count) { ammo = count; }
	FORCEINLINE void SetAttckPower(int32 damage) { attackPower = damage; }
public:
UPROPERTY(EditAnyWhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ABaseWeapon> _weapon;

//무기 장착
	UPROPERTY()
	class ABaseWeapon* _equipWeapon;

	//애니메이션
	UPROPERTY(EditAnywhere,Category = Anim)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = Anim)
	class UAnimMontage* ReloadMontage;

	/*UPROPERTY()
	class USH_Neon_AnimInstance* anim;*/

	UPROPERTY(EditDefaultsOnly, Category= MySettings)
	TSubclassOf<class UCameraShakeBase> FireShake;
	
	//원래 쐈던 위치 저장
	UPROPERTY(EditAnywhere,Category = "Fire")
	FRotator startFireRot;
	
	UPROPERTY(EditAnywhere,Category = "Fire") 
	bool isFireStart = false;

	//연사모드
	UPROPERTY(EditAnywhere, Replicated, Category = "Fire")
	bool isFire = false; 

	//총기 반동
	UPROPERTY(EditAnywhere,Category = "Fire")
	float RecoilAmount = 0;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float curRecoilAmount =0;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float curRightRecoilAmount =1;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float MaxTopRecoil = 5;

	UPROPERTY(Replicated)
	int32 health = 0;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	class ABaseWeapon* owningWeapon;

	UPROPERTY()
	class UFireUserWidget* fire_UI;

	bool bInDelay = false;
	bool bInSniperDelay = false;
	FTimerHandle fireDelay;
	FTimerHandle SniperDelay;

	FOnAmmoChangedDel OnAmmoCntChanged;

	UPROPERTY(EditAnywhere, Replicated)
		int32 ammo = 30;
		int32 FullAmmoCount = ammo;

	UPROPERTY(EditAnywhere, Replicated)
		int32 attackPower = 5;
	UPROPERTY(EditAnywhere, Replicated)
		int32 SniperPower = 50;

	UPROPERTY(EditAnywhere)
	float FireDelayTime = 0.1;
	UPROPERTY(EditAnywhere)
	float SniperDelayTime = 0.8;

	UPROPERTY(Replicated)
	bool bReloadOn;
	UFUNCTION(Server, Reliable)
	void ServerReloadComplete();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReloadComplete(int32 AmmoCnt);
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(bool InFireReloadChk, int32 CurrAmmoCnt);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSniperShot(bool InReloadOn, int32 CurrAmmoCnt);

	UFUNCTION(Server, reliable)
	void ServerFireEffect(class ABaseWeapon* Gun, const USkeletalMeshSocket* FireSocket,FVector p1, FVector p2, FRotator p3, FVector p4,bool bBlockingHit);

	UFUNCTION(NetMulticast, reliable)
	void MulticastFireEffect(class ABaseWeapon* Gun, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit);

	UFUNCTION(Server, Reliable)
	void ServerSniperEffect(class ABaseWeapon* Gun, const USkeletalMeshSocket* FireSocket,FVector p1, FVector p2, FRotator p3, FVector p4,bool bBlockingHit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSniperEffect(class ABaseWeapon* Gun, FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit);

	UFUNCTION(Server, Unreliable)
	void ServerHitProcess();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitProcess();
	void StopFire();
	UFUNCTION(Server, Reliable)
	void ServerStopFire();

		// 파티클
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;
	UPROPERTY(EditAnywhere)
	class USoundCue* SniperSound;

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound();

	void SetAmmoCountTextInit(class USkillWidget* SkillUI);

public:
	void SniperShot();
	UFUNCTION(Server, Reliable)
	void ServerSniperShot();

	APlayerController* playerController;

};
