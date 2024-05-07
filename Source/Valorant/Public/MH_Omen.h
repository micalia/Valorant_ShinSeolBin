// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "MH_Omen.generated.h"

/**
 * 
 */
//UENUM(BlueprintType)
//enum class ESkill_EMode : uint8
//{
//    ModeA UMETA(DisplayName = "Camera_Mode"),
//    ModeB UMETA(DisplayName = "Player_Mode"),
//};

UCLASS()
class VALORANT_API AMH_Omen : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMH_Omen();

	//componenet
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* NSHLeftPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* NSHRightPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* CM;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerFireComponent* PlayFire;

	//variable


	//---------
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//float Speed = 800;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//float MaxDistance = 3000;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//int32 Qcnt = 1;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsQ = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsC = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsE = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsForward = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsBackward = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool Impacted=false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsBlocking =false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//bool IsFire =false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//FVector ImpactPoint;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//float CameraToMarbleDistance=400;
	//---------

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Settings)
	//class ALeft_NSH* SpawnedNSH;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Settings)
	//class ANasunhwan* SpawnedENSH;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Settings)
	//TSubclassOf<class ANasunhwan> SpawnedNSHFactory;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystem* SpawnNSHEm;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystem* NSHEmitter;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystem* GuideLineEm;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	//class UParticleSystemComponent* GuideLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class USoundBase* SoundCcast0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class USoundBase* SoundCcast1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	class USoundBase* SoundEcast2;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Settings)
	//ESkill_EMode ESkill_Enum = ESkill_EMode::ModeA;

	//UFUNCTION(BlueprintCallable, Category = Function)
	//void SwitchMode(ESkill_EMode Mode);
	//UFUNCTION(BlueprintCallable, Category = Function)
	//void CameraSwitching();

	////Blueprint Implementable,,
	//UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = Function)
	//void CameraModeLevel();

	////Blueprint Implementable,,
	//UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = Function)
	//void PlayerModeLevel();

	//widget 
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UMainWidget> mainWidget;

	class UMainWidget* main_UI;

//public:
//	UFUNCTION(BlueprintCallable,Category="Function")
//	void CameraToFC();
//	UFUNCTION(BlueprintCallable,Category="Function")
//	void CameraToCM();
//	UFUNCTION(BlueprintPure,Category="Function")
//	FVector CameraPos();
//	UFUNCTION(BlueprintPure,Category="Function")
//	float Forwarding();
//	UFUNCTION(BlueprintPure,Category="Function")
//	float Backwarding();
//	UFUNCTION(BlueprintPure,Category="Function")
//	float distCameraToSphere();

	//UFUNCTION(BlueprintCallable, Category = "Function")
	//void Active();

	//UPROPERTY(EditAnywhere,Category=Settings)
	//class AQ_NSH* NSH;

	//UFUNCTION(BlueprintCallable, Category = "MyDelegate")
	//	void MyDestroyed(AActor* nshParam);
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

protected:
	//virtual void KeyQ();
	//virtual void KeyC();
	//virtual void KeyE();
	//virtual void MouseLeftAction();
	//virtual void MouseLeftReleasedAction();

	//virtual void R();
	//virtual void MouseRightAction();
	//virtual void MouseRightReleasedAction();
private:
	void PrintLog();

	enum ENetRole myLocalRole;
	enum ENetRole myRemoteRole;

	UPROPERTY(Replicated)
	float timeTest = 0;
};
