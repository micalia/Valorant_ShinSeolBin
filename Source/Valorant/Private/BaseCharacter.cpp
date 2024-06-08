// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include "PlayerFireComponent.h"
#include "Net/UnrealNetwork.h"
#include <GameFramework/PlayerController.h>
#include "EngineUtils.h"
#include "WinLoseWidget.h"
#include <GameFramework/PlayerState.h>
#include "FadeOut.h"
#include "NetPlayerController.h"
#include "NetGameInstance.h"
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FireUserWidget.h"
#include "Engine/World.h"
#include "BattleGameModeBase.h"
#include <Kismet/KismetMathLibrary.h>
#include <Sound/SoundBase.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "NetGameStateBase.h"
#include "SB_FireComponent.h"
#include "SB_Sova.h"
#include "SkillWidget.h"
#include "Components/Image.h"

//////////////////////////////////////////////////////////////////////////
// ABaseCharacter

ABaseCharacter::ABaseCharacter()
{
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...   
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetRelativeLocation(FVector(5, 0, 68));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character   
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	fpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("fpsMesh"));
	fpsMesh->SetupAttachment(GetCapsuleComponent());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

#pragma region Constructor
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (tempIMC.Succeeded())
	{
		DefaultMappingContext = tempIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_Move(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (tempIA_Move.Succeeded())
	{
		MoveAction = tempIA_Move.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_Look(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (tempIA_Look.Succeeded())
	{
		LookAction = tempIA_Look.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (tempIA_Jump.Succeeded())
	{
		JumpAction = tempIA_Jump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempMouseLeftAction(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_MouseLeft.IA_MouseLeft'"));
	if (tempMouseLeftAction.Succeeded())
	{
		mouseLeftAction = tempMouseLeftAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempMouseRightAction(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_MouseRight.IA_MouseRight'"));
	if (tempMouseRightAction.Succeeded())
	{
		mouseRightAction = tempMouseRightAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_Q_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_Q.IA_Key_Q'"));
	if (tempKey_Q_action.Succeeded())
	{
		Key_Q_action = tempKey_Q_action.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_E_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_E.IA_Key_E'"));
	if (tempKey_E_action.Succeeded())
	{
		Key_E_action = tempKey_E_action.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_X_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_X.IA_Key_X'"));
	if (tempKey_X_action.Succeeded())
	{
		Key_X_action = tempKey_X_action.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_C_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_C.IA_Key_C'"));
	if (tempKey_C_action.Succeeded())
	{
		Key_C_action = tempKey_C_action.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_F_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_F.IA_Key_F'"));
	if (tempKey_F_action.Succeeded())
	{
		Key_F_action = tempKey_F_action.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_R_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Key_R.IA_Key_R'"));
	if (tempKey_R_action.Succeeded())
	{
		Key_R_action = tempKey_R_action.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_V_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Skill_V.IA_Skill_V'"));
	if (tempKey_V_action.Succeeded())
	{
		Key_V_action = tempKey_V_action.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_G_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Skill_G.IA_Skill_G'"));
	if (tempKey_G_action.Succeeded())
	{
		Key_G_action = tempKey_G_action.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempKey_B_action(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Skill_B.IA_Skill_B'"));
	if (tempKey_B_action.Succeeded())
	{
		Key_B_action = tempKey_B_action.Object;
	}

	static ConstructorHelpers::FClassFinder<UWinLoseWidget> tempWinLoseWidgetFactory(TEXT("/Game/PSH/UI/WB_winLose.WB_winLose_C"));
	if (tempWinLoseWidgetFactory.Succeeded()) {
		WinLoseWidgetFactory = tempWinLoseWidgetFactory.Class;
	}


#pragma endregion
}

void ABaseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	soundKill = LoadObject<USoundBase>(nullptr, TEXT("/Game/LMH/Sounds/1stKill.1stKill"));
	soundShot = LoadObject<USoundBase>(nullptr, TEXT("/Game/LMH/Sounds/Body-Shot-1.Body-Shot-1"));
	soundV = LoadObject<USoundBase>(nullptr, TEXT("/Game/LMH/Sounds/09-Victory.09-Victory"));
	FollowCamera->SetPostProcessBlendWeight(0.0f);
	CurrHP = FullHP;

	//Add Input Mapping Context
	APlayerController* pc = GetController<APlayerController>();
	UE_LOG(LogTemp, Warning, TEXT("%s"), pc == nullptr ? *FString("NULL") : *FString("PC Have"));


	if (pc != nullptr)
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	fireComp = Cast<UPlayerFireComponent>(GetComponentByClass(UPlayerFireComponent::StaticClass()));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	if (IsLocallyControlled() == false) return;
	Super::Tick(DeltaTime);
	APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();
	int32 ControllerIndex = MyPlayerController->GetLocalPlayer()->GetControllerId();

	if (TheEndGame) {
		if (!TheEndGameOn) {
			TArray<UUserWidget*> ActiveWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ActiveWidgets, UUserWidget::StaticClass(), false);

			for (UUserWidget* Widget : ActiveWidgets)
			{
				if (!Widget->IsA(UWinLoseWidget::StaticClass()))
				{
					Widget->RemoveFromParent();
				}
			}

			TheEndGameOn = true;
		}
	}

	if (CurrHP <= 0) {

		if (!bDieOn) {
			if (soundKill) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundKill, GetActorLocation());
			}
			if (GetController() != nullptr)
			{
				if (GetController()->IsLocalPlayerController())
				{
					GetMesh()->SetVisibility(true);
					fpsMesh->SetVisibility(false);
				}
				if (HasAuthority()) {
					ServerDie_Implementation();
				}
				else {
					ServerDie();
				}
				DieProcess();

				bDieOn = true;
			}
		}
	}
}
void ABaseCharacter::ServerWin_Implementation()
{
	winloseInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Multicast_Win_Implementation();
}

void ABaseCharacter::Multicast_Win_Implementation()
{

	winloseInstance->StartWinAnim();
}

void ABaseCharacter::ServerLose_Implementation()
{
	winloseInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Multicast_Lose_Implementation();
}

void ABaseCharacter::Multicast_Lose_Implementation()
{
	winloseInstance->StartLoseAnim();
}

void ABaseCharacter::DefaultShootPress()
{
	if (fireComp) {
		fireComp->isFire = true;
	}
}

void ABaseCharacter::DefaultShootRelease()
{
	if (fireComp) {
		fireComp->StopFire();
	}
}

void ABaseCharacter::ServerDamagedHealth_Implementation(int32 value, ABaseCharacter* WhoKilledMe)
{
	MulticastAttackEnemyIndicator();

	CurrHP = CurrHP - value;

	if (CurrHP <= 0) {
		if (HasAuthority()) {
			if (WhoKilledMe) {
				WhoKilledMe->GetPlayerState()->SetScore(WhoKilledMe->GetPlayerState()->GetScore() + 1);

				if (WhoKilledMe->GetPlayerState()->GetScore() >= 3)
				{
					TheEndGame = true;
					WhoKilledMe->TheEndGame = true; // ºγ !!
					ANetGameStateBase* sb = GetWorld()->GetGameState<ANetGameStateBase>();
					sb->endGame = true;
					if (HasAuthority()) {
						Server_EndGame_Implementation();
					}
					else {
						Server_EndGame();
					}
				}
			}
			ServerDie_Implementation();
		}
		else ServerDie();

		FTimerHandle WinLoseTimer;
		GetWorld()->GetTimerManager().SetTimer(WinLoseTimer, FTimerDelegate::CreateLambda([&]() {
			if (HasAuthority()) {
				Server_CheckWinLose_Implementation();
			}
			else {
				Server_CheckWinLose();
			}

			}), 0.1f, false);
	}
}



void ABaseCharacter::Server_AllEndGame_Implementation()
{
	Multicast_AllEndGame_Implementation();
}

void ABaseCharacter::Multicast_AllEndGame_Implementation() {

}


void ABaseCharacter::Stun()
{
	APlayerController* pc = GetController<APlayerController>();

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	FollowCamera->SetPostProcessBlendWeight(1.0f);
	FollowCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);

	FTimerHandle recoverTime;
	GetWorld()->GetTimerManager().SetTimer(recoverTime, FTimerDelegate::CreateLambda([&]() {
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		FollowCamera->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
		FollowCamera->SetPostProcessBlendWeight(0.0f);
		}), 3.0f, false);
}

void ABaseCharacter::MulticastAttackEnemyIndicator_Implementation()
{
	if (IsLocallyControlled()) {
		if (ASB_Sova* Sova = Cast<ASB_Sova>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
			if (Sova->skillWigetInstance) {
				Sova->skillWigetInstance->DamageIndicator->SetRenderOpacity(1);
				Sova->skillWigetInstance->bActiveDamagedUI = true;
				Sova->skillWigetInstance->DamageUiActiveCurrTime = 0;
			}
		}
	}
}

void ABaseCharacter::PlayDieAnimationInWidget()
{
	FadeOutUI = CreateWidget<UFadeOut>(GetWorld(), FadeOutFactory);

	if (FadeOutUI)
	{
		FadeOutUI->AddToViewport();
	}
}

void ABaseCharacter::Server_WinLose_Implementation()
{
	winloseInstance->SetVisibility(ESlateVisibility::Visible);
	Multicast_WinLose_Implementation();
}

void ABaseCharacter::Multicast_WinLose_Implementation()
{
	winloseInstance->SetVisibility(ESlateVisibility::Visible);
}
void ABaseCharacter::DieProcess()
{
	if (HasAuthority()) {
		Server_NoCollision_Implementation();
	}
	else {
		Server_NoCollision();
	}

	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		DisableInput(GetController<APlayerController>());

		TArray<UUserWidget*> ActiveWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ActiveWidgets, UUserWidget::StaticClass(), false);

		for (UUserWidget* Widget : ActiveWidgets)
		{
			if (!Widget->IsA(UWinLoseWidget::StaticClass()))
			{
				Widget->RemoveFromParent();
			}
		}
	}
}
void ABaseCharacter::ServerDie_Implementation()
{
	Multicast_ServerDie();
}

void ABaseCharacter::RestartProcess()
{
	if (ANetPlayerController* NetPC = GetController<ANetPlayerController>())
	{
		if (HasAuthority()) {
			NetPC->ServerChangePlayerToSpectator_Implementation();
		}
	}
}

void ABaseCharacter::Multicast_RestartControlMode_Implementation(class ANetPlayerController* NetPC)
{
	NetPC->SetShowMouseCursor(false);

	FInputModeGameOnly inputMode = FInputModeGameOnly();

	NetPC->SetInputMode(inputMode);
}

void ABaseCharacter::Server_NoCollision_Implementation()
{
	if (HasAuthority()) {
		Multicast_NoCollision_Implementation();
	}
	else {
		Multicast_NoCollision();
	}
}

void ABaseCharacter::Multicast_NoCollision_Implementation()
{
	if (CurrHP <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, FString::Printf(TEXT("Multicast_NoCollision_Implementation")), true, FVector2D(1, 1));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::InitInputSetting(class APlayerController* con)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(con->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ABaseCharacter::Server_CheckWinLose_Implementation()
{
	Multicast_CheckWinLose();
}

void ABaseCharacter::Multicast_CheckWinLose_Implementation()
{
	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), playerArray);

	for (int i = 0; i < playerArray.Num(); i++)
	{
		ABaseCharacter* p = Cast<ABaseCharacter>(playerArray[i]);

		if (p->GetController() != nullptr && p->IsLocallyControlled() == true) {
			if (p->CurrHP <= 20) {
				if (!TheEndGame) {
					p->winloseInstance->StartLoseAnim();
				}
				else {
					p->winloseInstance->StartLoseEndingAnim();
				}
			}
			else {
				if (!TheEndGame) {
					p->winloseInstance->StartWinAnim();
				}
				else {
					p->winloseInstance->StartWinEndingAnim();
				}
			}
		}
	}

}


void ABaseCharacter::NotifyRestarted()
{
	Super::NotifyRestarted();
	if (WinLoseWidgetFactory) {
		if (IsLocallyControlled()) {
			TArray<UUserWidget*> ActiveWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ActiveWidgets, UUserWidget::StaticClass(), false);

			for (UUserWidget* Widget : ActiveWidgets)
			{
				if (!Widget->IsA(UWinLoseWidget::StaticClass()))
				{
					Widget->RemoveFromParent();
				}
			}
		}

		if (IsLocallyControlled()) {
			winloseInstance = CreateWidget<UWinLoseWidget>(GetWorld(), WinLoseWidgetFactory);
			if (winloseInstance) {
				winloseInstance->AddToViewport(1);
			}

		}
	}
}


void ABaseCharacter::Server_EndGame_Implementation()
{
	Multicast_EndGame_Implementation();
}

void ABaseCharacter::Multicast_EndGame_Implementation()
{
	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), playerArray);

	for (int i = 0; i < playerArray.Num(); i++)
	{
		ABaseCharacter* p = Cast<ABaseCharacter>(playerArray[i]);
		p->TheEndGame = true;
	}
}

void ABaseCharacter::Multicast_ServerDie_Implementation()
{
	PlayDieMontage();
}



void ABaseCharacter::ServerAddHealth_Implementation(int32 value)
{
	CurrHP = FMath::Min(CurrHP + value, FullHP);
}
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		EnhancedInputComponent->BindAction(mouseLeftAction, ETriggerEvent::Started, this, &ABaseCharacter::MouseLeftAction);
		EnhancedInputComponent->BindAction(mouseLeftAction, ETriggerEvent::Completed, this, &ABaseCharacter::MouseLeftReleasedAction);
		EnhancedInputComponent->BindAction(mouseRightAction, ETriggerEvent::Started, this, &ABaseCharacter::MouseRightAction);
		EnhancedInputComponent->BindAction(mouseRightAction, ETriggerEvent::Completed, this, &ABaseCharacter::MouseRightReleasedAction);

		EnhancedInputComponent->BindAction(Key_Q_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyQ);
		EnhancedInputComponent->BindAction(Key_E_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyE);
		EnhancedInputComponent->BindAction(Key_X_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyX);
		EnhancedInputComponent->BindAction(Key_C_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyC);
		EnhancedInputComponent->BindAction(Key_F_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyF);
		EnhancedInputComponent->BindAction(Key_R_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyR);
		EnhancedInputComponent->BindAction(Key_V_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyV);
		EnhancedInputComponent->BindAction(Key_G_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyG);
		EnhancedInputComponent->BindAction(Key_B_action, ETriggerEvent::Started, this, &ABaseCharacter::KeyB);

	}

	onInputBindingDelegate.Broadcast(PlayerInputComponent);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::MouseLeftAction()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("LeftClick")), true, FVector2D(1, 1));
}

void ABaseCharacter::MouseLeftReleasedAction()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("LeftClick Released")), true, FVector2D(1, 1));
}

void ABaseCharacter::MouseRightReleasedAction()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("RigtClick Released")), true, FVector2D(1, 1));
}

void ABaseCharacter::MouseRightAction()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("RightClick")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyQ()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyQ")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyE()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyE")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyX()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyX")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyC()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyC")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyF()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyF")), true, FVector2D(1, 1));
}
void ABaseCharacter::KeyR()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyF")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyV()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyV")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyG()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyG")), true, FVector2D(1, 1));
}

void ABaseCharacter::KeyB()
{
	if (IsLocallyControlled() == false) return;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("KeyB")), true, FVector2D(1, 1));
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrHP);
	DOREPLIFETIME(ABaseCharacter, bDie);
	DOREPLIFETIME(ABaseCharacter, TheEndGame);
	DOREPLIFETIME(ABaseCharacter, TheEndGameOn);
	DOREPLIFETIME(ABaseCharacter, Score);
	DOREPLIFETIME(ABaseCharacter, bDieOn);

}