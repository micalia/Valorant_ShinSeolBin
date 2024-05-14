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
	MinhaTeleport,
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
	virtual void KeyE() override; // 정찰용 화살
	virtual void KeyQ() override; // 수류탄
	virtual void KeyC() override; // 공중 연막
	virtual void KeyF() override; // MH 스킬
	virtual void KeyV() override;
	virtual void KeyG() override;
	virtual void KeyB() override;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASH_Neon> neonFactory;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool isGun = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* projectileComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* projectilePath;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* arrowMesh;
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

	void InitScoutingArrow();

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();
	void ScoutingArrowShot();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnArrow(class APlayerController* MyPlayer, FTransform transform, int32 bounceCount);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MultiCast_SpawnArrow(FTransform transform, int32 bounceCount);

	UPROPERTY(EditAnywhere)
	//파워 게이지 차는 속도
	float gaugeTime = 1;
	float gaugeCurrTime;
	class USoundBase* soundCharging;

	void ArrowPowerGaugeUp();

	UPROPERTY(EditAnywhere, Category = "ScoutingArrow")
	float minScoutingArrowSpeed = 2500;
	UPROPERTY(EditAnywhere, Category = "ScoutingArrow")
	float maxScoutingArrowSpeed = 8500;
	UPROPERTY(BlueprintReadOnly, Category = "ScoutingArrow", Replicated)
	float scoutingArrowSpeed;
	UFUNCTION(Server, Reliable)
	void Server_SetFloatScoutingArrow(float arrowSpeedAlpha);
	UFUNCTION(NetMulticast, Reliable)
	void Mulitcast_SetFloatScoutingArrow(float arrowSpeedAlpha);
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
	class UMaterialInterface* DefaultMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UStaticMesh* DefalutMesh;
	UPROPERTY(BlueprintReadWrite)
	TArray<class USplineMeshComponent*> SplineMeshComponents;
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool bThrowing;
	UFUNCTION(Server, Reliable)
	void ServerGrenadeThrowAction();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGrenadeThrowAction();

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
	UFUNCTION(Server,Reliable)
	void ServerCancelGrenade();

	// 수류탄 생성
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenade> GrenadeFactory;
	AGrenade* MyGrenade;
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

	//********MH 순간이동*********//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FVector ImpactPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	bool Impacted = false;


	//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystemComponent* GuideLine;

	//test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class UNiagaraComponent* NGGuideLine;
	//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystem* GuideLineEm;

	//test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class UNiagaraSystem* NGGuideLineEm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float MaxDistance = 3000;

	UFUNCTION(Server, Reliable)
	void Server_SetLocation(FVector pos);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetLocation(FVector pos);

	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UFireUserWidget> fireWidget;
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UUserWidget> smokeSkillUIFactory;
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UUserWidget> ProtoSmokeSkillUIFactory;

	UPROPERTY()
	class UFireUserWidget* fire_UI;

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
};
