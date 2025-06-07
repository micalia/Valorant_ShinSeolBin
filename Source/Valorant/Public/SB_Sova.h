// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SB_Sova.generated.h"

UENUM(BlueprintType)
enum class ESovaState :uint8
{
	Reload,
	DefaultAtk,
	ScoutingArrow,
	AirSmoke,
	Grenade,
	DragonStrike
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRemoveSmokeMarkerUI);
UCLASS()
class VALORANT_API ASB_Sova : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASB_Sova();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MouseLeftAction() override;
	virtual void MouseLeftReleasedAction() override;
	virtual void MouseRightAction() override;
	virtual void MouseRightReleasedAction() override;
	virtual void KeyE() override; // ������ ȭ��
	virtual void KeyQ() override; // ����ź
	virtual void KeyC() override; // ���� ����
	virtual void KeyF() override; // ���� �̵�
	virtual void KeyR() override; // ���� �ϰ�
	virtual void KeyV() override; // ���� �ϰ�
	virtual void PossessedBy(AController* NewController) override;
public:
//ī�޶� ����ũ
	UPROPERTY(EditAnywhere)
	float cameraShakeTime = 0.1;
	float camCurrTime;

	FVector cameraOriginPos;
	FRotator cameraOriginRot;
	void CameraShakeRandom();

	APlayerCameraManager* PlayerCam;
//********************************
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool isGun = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* projectileComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* projectilePath;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* arrowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* BaseWeapon;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> BaseWeaponFactory;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* DeathCamera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DeathCameraFactory;

	virtual void PlayDieMontage() override;

	UPROPERTY()
	class UAnimMontage* DieMontage;

	virtual void MeshVisible() override;
public:
	UPROPERTY(BlueprintReadOnly)
	float h;
	UPROPERTY(BlueprintReadOnly)
	float v;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USkillWidget> SkillWidgetFactory;
	UPROPERTY()
	class USkillWidget* skillWigetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ESovaState currState = ESovaState::DefaultAtk;

	void ChangeCurrState(ESovaState InCurrState);

	FORCEINLINE ESovaState GetCurrState(){return currState;}

	UFUNCTION(Server, Reliable)
	void Server_SetCurrState(ESovaState paramCurrState);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCurrState(ESovaState paramCurrState);

	bool mouseLeftClick;

	UFUNCTION(BlueprintImplementableEvent)
	void SniperRecoilCameraEffect();

	bool bSniperOn = false;
	void SetSniperOn(bool InSniperOn);
	FORCEINLINE bool GetSniperOn(){return bSniperOn;}

	UPROPERTY(BlueprintReadWrite)
	bool bSniperRecoil = false;

	void CoolTimeCalculate(float InDeltaTime);
	void SetCoolTimeUI();

	void SkillCoolTimeInit();

	//********������ȭ��********//
	UPROPERTY(EditAnywhere)
	float ScoutingArrowCoolTime = 5;
	float ScoutingArrowCoolCurrTime = ScoutingArrowCoolTime;
	
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice1;
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice2;
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice3;

	UPROPERTY()
	class UAnimMontage* OnlyArrowMontage;
	UPROPERTY()
	class UAnimInstance* OnlyArrowABP;

	UPROPERTY(EditAnywhere)
	class USceneComponent* ArrowFirePos;
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bScoutingArrow;
	UFUNCTION(Server, Reliable)
	void Server_SetBoolScoutingArrow(bool bScoutingChk);
	UFUNCTION(NetMulticast, Reliable)
	void Mulitcast_SetBoolScoutingArrow(bool bScoutingChk);
private:
	bool bCanScouting = false;
public:
	void SetBoolScoutingArrow(bool InScoutingState){bCanScouting = InScoutingState;}
	bool GetBoolScoutingArrow(){return bCanScouting;}

	UPROPERTY(Replicated)
	class ASB_ArrowVersion2* CurrArrow;

	TSubclassOf<class ASB_Arrow> arrowFactory;
	TSubclassOf<class ASB_ArrowVersion2> ArrowVer2Factory;

	void InitScoutingArrow();

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();
	FVector GetArrowDirVec();
	void PlayShotVoice();

	UFUNCTION(Server, Reliable)
	void Server_SpawnArrow(class ABaseCharacter* MyPlayer);
	UFUNCTION(Server, Reliable)
	void Server_DestroyArrow();

	UPROPERTY(EditAnywhere)
	//�Ŀ� ������ ���� �ӵ�
	float gaugeTime = 1;
	float gaugeCurrTime;

	void ArrowPowerGaugeUp();

	UPROPERTY(EditAnywhere, Category = "ScoutingArrow")
	float minScoutingArrowSpeed = 2150;
	UPROPERTY(EditAnywhere, Category = "ScoutingArrow")
	float maxScoutingArrowSpeed = 3100;
	UPROPERTY(BlueprintReadOnly, Category = "ScoutingArrow", Replicated)
	float scoutingArrowSpeed;
	UPROPERTY(BlueprintReadOnly)
	int32 skillBounceCount = 0;
	void IncreaseBounceCount();

	UPROPERTY()
	TSubclassOf<class UUI_SB_ScoutingArrow> ui_SB_ScoutingArrowFactory;
	UPROPERTY()
	class UUI_SB_ScoutingArrow* ui_SB_ScoutingArrowInstance;

	FTimerHandle DestroyArrowDelayHandle;
	//********����ź********//
	UPROPERTY(EditAnywhere)
	float GrenadeCoolTime = 5;
	float GrenadeCoolCurrTime = GrenadeCoolTime;
	//��Ÿ��
	UPROPERTY()
	class UAnimMontage* SovaGrenadeMongtage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UMaterialInterface* GrenadeSplineMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UStaticMesh* GrenadeSplineMesh;
	UPROPERTY(BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshComponents;
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bThrowing;
	UFUNCTION(Server, Reliable)
	void ServerGrenadeThrowAction();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGrenadeThrowAction(bool bThrow);

	UPROPERTY(EditAnywhere)
	float ThrowPower = 1700;

	// ���� �غ�
	UFUNCTION(Server, Reliable)
	void ServerGrenadeThrowReady(APlayerController* MyPlayerController);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGrenadeThrowReady(APlayerController* MyPlayerController);
	void ShowProjectilePath();
	FVector GetThrowVelocity();

	UFUNCTION(BlueprintImplementableEvent)
	void GrenadeThrowAction();
	UFUNCTION()
	void ClearPath();
	UFUNCTION(Server, Reliable)
	void ServerCancelGrenade();

	// ����ź ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenade> GrenadeFactory;
	class AGrenade* MyGrenade;
	UFUNCTION(Server, Reliable)
	void ServerSpawnGrenade(APlayerController* MyPlayerController);

	UPROPERTY(EditAnywhere)
	class USoundBase* GrenadeSound1;
	UPROPERTY(EditAnywhere)
	class USoundBase* GrenadeSound2;

	//********���߿���********//
	UPROPERTY(EditAnywhere)
	float AirSmokeCoolTime = 5;
	float AirSmokeCoolCurrTime = AirSmokeCoolTime;
	UPROPERTY()
	class USoundBase* SovaAirSmokeVoice1;
	UPROPERTY()
	class USoundBase* SovaAirSmokeVoice2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bProtoMapSkillMode = false;

	FRemoveSmokeMarkerUI OnRemoveSmokerUI;
	UPROPERTY(BlueprintReadWrite)
	class USB_AirSmokeMarker* AirSmokeMarkerRef;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SmokeObjFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USB_AirSmokeMarker> AirSmokeMarkerFactory;

	UPROPERTY(BlueprintReadWrite)
	class APlayerController* PlayerRef;
	UFUNCTION()
	void ActiveAirSmoke();
	UFUNCTION(BlueprintCallable)
	void DeactiveAirSmoke();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 airSmokeMaxCount = 3;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 airSmokeCurrCount;

	void AirSmokeLogic();
	void AirSmokeVoice();
	UFUNCTION(Server, Reliable)
	void ServerSpawnSmokeObj(FVector SpawnPos);

	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UUserWidget> smokeSkillUIFactory;
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UUserWidget> ProtoSmokeSkillUIFactory;

	UPROPERTY()
	class UFireUserWidget* fire_UI;

	void AirSmokeMouseCursor();
public:
	UPROPERTY()
	class USoundBase* throwBump;
	UPROPERTY()
	class USoundBase* expl;

	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* smokeSkillUIinstance;

	virtual void NotifyRestarted() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DebugSphere();

public:
	//********���� �̵�********//
	UPROPERTY(EditAnywhere)
	float HookCoolTime = 2;
	float HookCoolCurrTime = HookCoolTime;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASB_Hook> HookActorFactory;

	UPROPERTY(EditAnywhere)
	class USoundBase* GrappleShotSound;

	UPROPERTY(EditAnywhere, Category = Grab, BlueprintReadWrite)
	class UCableComponent* CableComp;

	UPROPERTY()
	class ASB_Hook* MyHook;

	UPROPERTY(EditAnywhere, Category = "Materials")
    class UMaterialInterface* RopeMat;

	void HookNotify();

	FTimerHandle HookDelayHandle;
	FTimerHandle EndHookSkillHandle;

	UFUNCTION(Server, Reliable)
	void ServerGrappleAction();
	void AttachHook(class ASB_Hook* HookPtr);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEndGrappleAction();

	void EndGrappleMove();
	void GrappleAction();

	UPROPERTY(BlueprintReadOnly)
	FVector GrappleEndLoc;

	UPROPERTY(BlueprintReadOnly);
	float ToGrappleDeltaTime;

	bool bThrowHook;

	UPROPERTY(EditAnywhere)
	float GrappleHookDistance = 3000;
	
	UPROPERTY(BlueprintReadOnly);
	FVector HookLocation;

	/*UPROPERTY(EditAnywhere)
	float GrappleHookCoolDown = 1;*/

	UFUNCTION(BlueprintImplementableEvent)
	void ShootGrappleHook();

	UPROPERTY(EditAnywhere)
	float GrappleMovePower = 2000000;
	
	UPROPERTY(EditAnywhere)
	float GrappleMoveStopDist = 400;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bCanGrappleAction = true;

	FTimerHandle GrappleMoveDelayHandle;
public:
	//********���� �ϰ�********//
	bool bDragonArrowOn = false;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASB_DragonArrow> DragonArrowFactory;

	UFUNCTION(Server, Reliable)
	void Server_SpawnDragonArrow(class ABaseCharacter* MyPlayer);
	UFUNCTION(Server, Reliable)
	void Server_DestroyDragonArrow();

	UPROPERTY(Replicated)
	class ASB_DragonArrow* CurrDragonArrow;

	UPROPERTY(EditAnywhere)
	class USoundBase* DragonStrikeVoice;

	UPROPERTY(EditAnywhere)
	class USoundBase* SuperSkillGaugeFullChargeSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* NotReadySuperSkill;

	virtual void SuperSkillGaugeUp(int32 DamageVal, class ABaseCharacter* WhoHitMe) override;
	void InitSuperSkillGauge();

	UFUNCTION(Server, Reliable)
	void Server_SetAvailableSuperSkill();
	UFUNCTION(Server, Reliable)
	void Server_SetNotAvailableSuperSkill();
	void UltimateAvailable();
	void UltimateGaugeInit();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSuperSkillGaugeUp(float InGaugeVal);

	FTimerHandle DragonStrikeStartDelayHandle;
	FTimerHandle DestroyDragonArrowDelayHandle;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = MySettings)
	bool bSovaCrounch = false;

	UFUNCTION(Server, Reliable)
	void ServerSetCrounch();
};
