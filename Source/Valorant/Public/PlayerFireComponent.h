// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFireComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UPlayerFireComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
/*
	UPlayerFireComponent();
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	void Reload();
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	//void BaseFire();
	
	//void WeaponInfoReset();
	FORCEINLINE void SetOwningWeapon(class ABaseWeapon* weapon) { owningWeapon = weapon; }
	FORCEINLINE class ABaseWeapon* GetOwningWeapon() const { return owningWeapon; }
	FORCEINLINE int32 GetAmmo() const { return ammo; }
	
	FORCEINLINE void SetAmmo(int32 count) { ammo = count; }
	FORCEINLINE void SetAttckPower(int32 damage) { attackPower = damage; }
	FORCEINLINE void SetFireInterval(float time) { fireInterval = time; }
	FORCEINLINE void Setmagazine(int32 count) { magazine = count; }
public:
UPROPERTY(EditAnyWhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ABaseWeapon> _weapon;

	//void AttachWeapon(TSubclassOf<class ABaseWeapon> weapon);

//무기 장착
	UPROPERTY()
	class ABaseWeapon* _equipWeapon;

	//애니메이션
	UPROPERTY(EditAnywhere,Category = Anim)
	class UAnimMontage* fireMontage;

	UPROPERTY(EditDefaultsOnly, Category= MySettings)
	TSubclassOf<class UCameraShakeBase> FireShake;
	
	//원래 쐈던 위치 저장
	UPROPERTY(EditAnywhere,Category = "Fire")
	FRotator startFireRot;
	
	UPROPERTY(EditAnywhere,Category = "Fire") //??
	bool isFireStart = false;

	//연사모드
	UPROPERTY(EditAnywhere,Category = "Fire")
	bool isFire = false; //?

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

	UPROPERTY(EditAnywhere, Replicated)
	class ABaseWeapon* owningWeapon;

	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UFireUserWidget> fireWidget;

	UPROPERTY()
	class UFireUserWidget* fire_UI;

	bool bInDelay = false;
	FTimerHandle fireDelay;

	UPROPERTY(EditAnywhere, Replicated)
		int32 ammo = 30;

	UPROPERTY(EditAnywhere, Replicated)
		int32 attackPower = 20;

	UPROPERTY(EditAnywhere, Replicated)
		int32 magazine = 3;

	UPROPERTY(EditAnywhere, Replicated)
		float fireInterval = 0.1f;

	void Fire();


	//UFUNCTION(Server, Unreliable)
	//	void ServerFireEffect();

	//UFUNCTION(NetMulticast, Unreliable)
	//	void MulticastFireEffect();

	//	UFUNCTION(Server, Unreliable)
	//void ServerFireEffect(const FHitResult& FireHit, const FVector& Start, const FVector& End);

	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastFireEffect();


	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerFire();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire();

	UFUNCTION(Server, reliable)
	void ServerFireEffect(const USkeletalMeshSocket* FireSocket,FVector p1, FVector p2, FRotator p3, FVector p4,bool bBlockingHit);

	UFUNCTION(NetMulticast, reliable)
	void MulticastFireEffect(FVector p1, FVector p2, FRotator p3, FVector p4, bool bBlockingHit);

	UFUNCTION(Server, Unreliable)
	void ServerHitProcess();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitProcess();

	//UFUNCTION(Server, Reliable)
	//	void ServerDamagedHealth(int32 value);

	//void DieProcess();

	//void StartFire();
	void StopFire();
	 
	//void OnHitWall(const FHitResult& hitResult); // 데칼 만들기 (벽에 맞으면 생기는 총자국)

		// 파티클
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

private:
	void PrintLog();
	enum ENetRole myLocalRole;
	enum ENetRole myRemoteRole;

	APlayerController* playerController;

	//USkeletalMeshSocket* FireSocket;
	//FTransform SocketTransform;
	//FVector StartTl;
	*/
};
