// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "RespawnWidget.h"
#include "SH_Neon.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ASH_Neon : public ABaseCharacter
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class APSH_Weapon> _weapon;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* NSHRightPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* NSHLeftPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* CM;

public:

	ASH_Neon();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MouseLeftAction() override;
	virtual void MouseLeftReleasedAction() override;
	virtual void MouseRightAction() override;

	virtual void KeyQ() override;
	virtual void KeyE() override;
	virtual void KeyX() override;
	virtual void KeyC() override;

	void WeaponInfoReset();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE void SetOwningWeapon(class APSH_Weapon* weapon) { owningWeapon = weapon; }
	FORCEINLINE class APSH_Weapon* GetOwningWeapon() const { return owningWeapon; }
	FORCEINLINE int32 GetAmmo() const { return ammo; }
	FORCEINLINE void SetAmmo(int32 count) { ammo = count; }
	FORCEINLINE void SetAttckPower(int32 damage) { attackPower = damage; }
	FORCEINLINE void SetFireInterval(float time) { fireInterval = time; }
	FORCEINLINE void Setmagazine(int32 count) { magazine = count; }
	FORCEINLINE float GetEnege() { return energy; };
public: 	
	void AttachWeapon(TSubclassOf<class APSH_Weapon> weapon);
	void Reload();

	UFUNCTION()
	void HitProcess();

	UPROPERTY(BlueprintReadWrite)
	bool HasWeapon = true;
	UPROPERTY()
	ABaseCharacter* me;
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	class USkeletalMeshComponent * T_meshcomp;

	UPROPERTY(EditAnywhere,Category = "Skill_Q")
	TSubclassOf<class APSH_Neon_RelayBolt>relayVolt_BulletFat;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	TSubclassOf<class APSH_Neon_FastLane>fastLaneFat;

	// 추월차선
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	TSubclassOf<class ASH_FastLane_OverlapActor>LaneOverlaFat;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector lefStartLoc;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector rightStartLoc;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector L_WallPos ;
	UPROPERTY(EditAnywhere,Category = "Skill_C")
	FVector R_WallPos ;

	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundCue * SkillQ;
	
	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundCue * SkillE;

	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundCue * SkillC;
	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundWave * FastLaneSound;
	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundWave * VoltSound;
	UPROPERTY(EditAnywhere,Category = "Sounds")
	class USoundWave * gearSound;
	
	UPROPERTY()
	class USH_Neon_AnimInstance* anim;
	class USH_Neon_AnimInstance* T_anim;
	// 카메라
	UPROPERTY(EditDefaultsOnly, Category= MySettings)
	TSubclassOf<class UCameraShakeBase> FireShake;

	//무기 장착
	UPROPERTY()
	class APSH_Weapon* _equipWeapon;

	//애니메이션
	UPROPERTY(EditAnywhere,Category = Anim)
	class UAnimMontage* fireMontage;
	
	//스킬 변수
	UPROPERTY(EditAnywhere,Category = "Skill_E")
	float UpSpeed = 10000;

	UPROPERTY(EditAnywhere,Category = "Skill_E")
	float curSpeed = 600;
	
	UPROPERTY(EditAnywhere,Category= "Skill_E")
	bool isHighGear = false;

	UPROPERTY(EditAnywhere,Category = "Skill_P")
	float energy = 100;

	UPROPERTY(EditAnywhere,Category = "Skill_P")
	float Maxenergy = 100;
	
	UPROPERTY(EditAnywhere,Category = "Skill_P")
	bool ischarge = false;


	// 총 발사 관련
	UPROPERTY(EditAnywhere,Category = "Fire")
	FRotator startFireRot;
	
	UPROPERTY(EditAnywhere,Category = "Fire")
	bool isFireStart = false;
	UPROPERTY(EditAnywhere,Category = "Fire")
	bool isFire = false;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float RecoilAmount = 0;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float curRecoilAmount =0;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float curRightRecoilAmount =1;
	UPROPERTY(EditAnywhere,Category = "Fire")
	float MaxTopRecoil = 5;

	UPROPERTY(EditAnywhere)
		int32 ammo = 0;

	UPROPERTY(EditAnywhere)
		int32 attackPower = 0;

	UPROPERTY(EditAnywhere)
		int32 magazine = 0;

		// 포스트 프로세스
	UPROPERTY(EditAnywhere)
	class UPostProcessComponent* post;
private:
	UPROPERTY(EditAnywhere)
	class APSH_Weapon* owningWeapon;

	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UFireUserWidget> fireWidget;
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UPSH_SkillWidget> SkillWidget;

public:
	UPROPERTY()
	class UFireUserWidget* fire_UI;

	UPROPERTY()
	class UPSH_SkillWidget* Skill_UI;

	bool bInDelay = false;
	FTimerHandle fireDelay;

	UPROPERTY(EditAnywhere)
	float fireInterval = 0.0f;

	void ReleaseWeapon();

// 	void StartFire(float deltaTime);
// 	void StopFire();

	UFUNCTION(Server,Unreliable)
	void SuverSpawnLane();
	UFUNCTION(NetMulticast,reliable)
	void Multisound();

	UFUNCTION(Server,Unreliable)
	void SeverSpawnVolt();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiSpawnVolt();

	void OnHitWall(const FHitResult& hitResult); // 데칼 만들기 (벽에 맞으면 생기는 총자국)

public:
//SB 
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Play_1P_Skill_Q_Animation();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Play_3P_Skill_C_Animation();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Play_3p_FireMontage();

//multy

	UFUNCTION(NetMulticast,Unreliable)
	void Skill_Q_Anim();
	UFUNCTION(NetMulticast,Unreliable)
	void Skill_C_Anim();
	UFUNCTION(NetMulticast,Unreliable)
	void Fire_Anim();

	virtual void NotifyRestarted() override;
};
