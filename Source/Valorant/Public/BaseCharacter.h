// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);

class UBoxComponent;

UCLASS()
class VALORANT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mouseLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* mouseRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_Q_action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_E_action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_X_action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_C_action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_F_action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_R_action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_V_action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_G_action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Key_B_action;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	class USoundBase* WinSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* DefeatSound;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void MouseLeftAction();
	virtual void MouseLeftReleasedAction();
	virtual void MouseRightReleasedAction();
	virtual void MouseRightAction();

	virtual void KeyQ();
	virtual void KeyE();
	virtual void KeyX();
	virtual void KeyC();
	virtual void KeyF();
	virtual void KeyR();
	virtual void KeyV();
	virtual void KeyG();
	virtual void KeyB();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FInputBindingDelegate onInputBindingDelegate;

	virtual void PlayFireAnimation() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* fpsMesh;

public:
	//Default Attack
	UPROPERTY()
	class UPlayerFireComponent* fireComp = nullptr;
	UFUNCTION(BlueprintCallable)
	void DefaultShootPress();

	UFUNCTION(Server, Reliable)
	void ServerDefaultShootPress();

	UFUNCTION(BlueprintCallable)
	void DefaultShootRelease();

	bool bCameraShake = false;
public:
	//stat

	FORCEINLINE int32 GetHP() const { return CurrHP; }

	UPROPERTY(EditAnywhere)
	float FullHP = 100;
	UPROPERTY(BlueprintReadWrite, Replicated)
	float CurrHP = 0;

	UFUNCTION(Server, Reliable)
	void ServerAddHealth(int32 value);

	UFUNCTION(Server, Reliable)
	void ServerDamagedHealth(int32 value, ABaseCharacter* WhoKilledMe, bool IsSuperSkill = false);

	//void DamagedHealth(int32 value, ABaseCharacter* WhoKilledMe);

	void Stun();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackEnemyIndicator();

	FORCEINLINE int32 GetHealth() { return CurrHP; };

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWinLoseWidget> WinLoseWidgetFactory;
	UPROPERTY(BlueprintReadOnly)
	class UWinLoseWidget* winloseInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFadeOut> FadeOutFactory;

	UFUNCTION(Server, Reliable)
	void Server_AllEndGame();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AllEndGame();

	UPROPERTY()
	class UFadeOut* FadeOutUI;

	void PlayDieAnimationInWidget();

	UPROPERTY(Replicated)
	bool bDie;
	UPROPERTY(Replicated)
	bool bDieOn = false;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool TheEndGame = false;


	UPROPERTY(BlueprintReadOnly, Replicated)
	bool TheEndGameOn = false;

	class ABattleGameModeBase* BM;

	UPROPERTY(Replicated)
	float Score = 0;

	UPROPERTY(EditAnywhere)
	int32 EndGameScore = 1;

	UFUNCTION(Server, Reliable)
	void Server_WinLose();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WinLose();

	UFUNCTION(Server, Reliable)
	void ServerDie();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ServerDie();


	//--------
	UFUNCTION(Server, Reliable)
	void ServerWin();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Win();

	UFUNCTION(Server, Reliable)
	void ServerLose();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Lose();

	//----------
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFpsMontage();

	virtual void PlayDieMontage();

	/*UFUNCTION()
	void OnDieProcess();*/

	UFUNCTION()
	void DieProcess();

	UFUNCTION()
	void RestartProcess();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RestartControlMode(class ANetPlayerController* NetPC);

	virtual void MeshVisible();

	UPROPERTY()
	class USoundBase* soundKill;
	UPROPERTY()
	class USoundBase* soundV;
	UPROPERTY()
	class USoundBase* soundShot;
	class ABaseCharacter* other;
public:
	UFUNCTION(Server, Reliable)
	void Server_NoCollision();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NoCollision();

	UFUNCTION(BlueprintCallable)
	void InitInputSetting(class APlayerController* con);

public:
	UFUNCTION(Server, Reliable)
	void Server_CheckWinLose();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CheckWinLose();


	virtual void NotifyRestarted() override;

	UFUNCTION(Server, Reliable)
	void Server_EndGame();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndGame();

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	float SuperSkillGauge = 0; // 1 = 100%

	virtual void SuperSkillGaugeUp(int32 DamageVal, class ABaseCharacter* WhoHitMe);

	UPROPERTY(Replicated)
	bool bAvailableSuperSKill;

public:
	UFUNCTION(BlueprintCallable)
	void LoadLobby();

private:
	float CheckZposTime = 2;
	float currCheckZposTime=0;

	void RandomSpawn();

protected:
	/** More actions
	* Hit boxes used for server-side rewind
	*/

	UPROPERTY(EditAnywhere)
	UBoxComponent* Head;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Spine3;

	UPROPERTY(EditAnywhere)
	UBoxComponent* L_Knee;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* L_Elbow;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* R_Elbow;

	UPROPERTY(EditAnywhere)
	UBoxComponent* R_Knee;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* L_Hip;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* R_Hip;
};
