// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SB_Sova.generated.h"

UENUM(BlueprintType)
enum class ESovaState :uint8
{
	DefaultAtk,
	ScoutingArrow,
	AirSmoke,
	Grenade,
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
	virtual void KeyE() override; // 정찰용 화살
	virtual void KeyQ() override; // 수류탄
	virtual void KeyC() override; // 공중 연막
	virtual void KeyF() override; // 로프 이동
	virtual void PossessedBy(AController* NewController) override;
public:
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

	UFUNCTION(Server, Reliable)
	void Server_SetCurrState(ESovaState paramCurrState);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCurrState(ESovaState paramCurrState);

	bool mouseLeftClick;

	UFUNCTION(BlueprintImplementableEvent)
	void SniperRecoilCameraEffect();

	bool bSniperOn = false;
	UPROPERTY(BlueprintReadWrite)
	bool bSniperRecoil = false;

	//********정찰용화살********//
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice1;
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice2;
	UPROPERTY()
	class USoundBase* ScoutingArrowVoice3;
	UPROPERTY(EditAnywhere)
	class USceneComponent* ArrowFirePos;
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bScoutingArrow;
	UFUNCTION(Server, Reliable)
	void Server_SetBoolScoutingArrow(bool bScoutingChk);
	UFUNCTION(NetMulticast, Reliable)
	void Mulitcast_SetBoolScoutingArrow(bool bScoutingChk);

	TSubclassOf<class ASB_Arrow> arrowFactory;
	TSubclassOf<class ASB_ArrowVersion2> ArrowVer2Factory;

	void InitScoutingArrow();

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();
	void ScoutingArrowShot();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnArrow(class APlayerController* MyPlayer, FTransform transform, int32 bounceCount, FVector InDirVec, float ArrowSpeed);

	UPROPERTY(EditAnywhere)
	//파워 게이지 차는 속도
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

	//********수류탄********//
	//몽타주
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
	void MulticastGrenadeThrowAction();

	UPROPERTY(EditAnywhere)
	float ThrowPower = 1700;

	// 던질 준비
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

	// 수류탄 생성
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenade> GrenadeFactory;
	class AGrenade* MyGrenade;
	UFUNCTION(Server, Reliable)
	void ServerSpawnGrenade(APlayerController* MyPlayerController);

	UPROPERTY(EditAnywhere)
	class USoundBase* GrenadeSound1;
	UPROPERTY(EditAnywhere)
	class USoundBase* GrenadeSound2;

	//********공중연막********//
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASB_Hook> HookActorFactory;

	UPROPERTY(EditAnywhere, Category = Grab, BlueprintReadWrite)
	class UCableComponent* CableComp;

	UPROPERTY()
	class ASB_Hook* MyHook;

	UPROPERTY(EditAnywhere, Category = "Materials")
    class UMaterialInterface* RopeMat;

	void HookNotify();

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

	UPROPERTY(EditAnywhere)
	float GrappleHookCoolDown = 1;

	UFUNCTION(BlueprintImplementableEvent)
	void ShootGrappleHook();

	UPROPERTY(EditAnywhere)
	float GrappleMovePower = 2000000;
	
	UPROPERTY(EditAnywhere)
	float GrappleMoveStopDist = 400;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bCanGrappleAction = true;
};
