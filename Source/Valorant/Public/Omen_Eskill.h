// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Omen_Eskill.generated.h"

UENUM(BlueprintType)
enum class ESkill_EMode : uint8
{
	ModeA UMETA(DisplayName = "Camera_Mode"),
	ModeB UMETA(DisplayName = "Player_Mode"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UOmen_Eskill : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOmen_Eskill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent);


public:	
	
public:
	UPROPERTY()
	class ABaseCharacter* me;

	class AMH_Omen* omen;
	class ASH_Neon* neon;
	class ASB_Sova* sova;

	class ABaseCharacter* Owner;

	//class A* me;

	//UPROPERTY()
	//class APlayerController* playerController;


	void LeftMouseAction();
	void LeftReleasedAction();
	void RightMouseAction();
	void RightReleasedAction();
	void KeyR();
	void KeyF();


//public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float Speed = 800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float MaxDistance = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	bool IsE = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	bool IsForward = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	bool IsBackward = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	bool IsBlocking = false;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
//	FVector ImpactPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	float CameraToMarbleDistance = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= MHSettings)
	class AMHnSB_NSH* SpawnedENSH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= MHSettings)
	TSubclassOf<class AMHnSB_NSH> SpawnedNSHFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	class UParticleSystem* SpawnNSHEm;
//--------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = MHFunction)
	void MyDestroyed(AActor* nshParam);
	UFUNCTION(BlueprintCallable, Category = MHFunction)
	void CameraSwitching();
	UFUNCTION(BlueprintCallable,Category= MHFunction)
	void CameraToFC();
	UFUNCTION(BlueprintCallable,Category= MHFunction)
	void CameraToCM();
	UFUNCTION(BlueprintCallable, Category = MHFunction)
	void SwitchMode(ESkill_EMode Mode);

		UFUNCTION(BlueprintPure,Category= MHFunction)
	FVector CameraPos();
	UFUNCTION(BlueprintPure,Category= MHFunction)
	float Forwarding();
	UFUNCTION(BlueprintPure,Category= MHFunction)
	float Backwarding();
	UFUNCTION(BlueprintPure,Category= MHFunction)
	float distCameraToSphere();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MHSettings)
	ESkill_EMode ESkill_Enum = ESkill_EMode::ModeA;

			//Blueprint Implementable,,
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = MHFunction)
	void CameraModeLevel();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = MHFunction)
	void PlayerModeLevel();
};
