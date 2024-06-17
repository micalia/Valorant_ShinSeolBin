// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_Sova.h"
#include "UI_SB_ScoutingArrow.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "SB_Arrow.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/SplineComponent.h>
#include "PlayerFireComponent.h"
#include "SB_SovaAnim.h"
#include <Components/CapsuleComponent.h>
#include "Net/UnrealNetwork.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include "SkillWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FireUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include <Sound/SoundBase.h>
#include "Kismet/KismetMathLibrary.h"
#include <AirSmokeMinimapWidget.h>
#include "SB_AirSmokeMarker.h"
#include "GameFramework/PlayerController.h"
#include "Components/SplineMeshComponent.h"
#include "Grenade.h"
#include "SB_ArrowVersion2.h"
#include "Components/ChildActorComponent.h"
#include "CableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../../Engine/Classes/GameFramework/SpringArmComponent.h"
#include "../../UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "../../Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "SB_Hook.h"

ASB_Sova::ASB_Sova()
{
	static ConstructorHelpers::FClassFinder<UUI_SB_ScoutingArrow> tempUI_SB_ScoutingArrowFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_ScoutingArrow.WB_ScoutingArrow_C'"));
	if (tempUI_SB_ScoutingArrowFactory.Succeeded()) {
		ui_SB_ScoutingArrowFactory = tempUI_SB_ScoutingArrowFactory.Class;
	}

	static ConstructorHelpers::FClassFinder<ASB_Arrow> tempArrowFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_Arrow.BP_Arrow_C'"));
	if (tempArrowFactory.Succeeded()) {
		arrowFactory = tempArrowFactory.Class;
	}
	static ConstructorHelpers::FClassFinder<ASB_ArrowVersion2> tempArrowVer2Factory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_ArrowVersion2.BP_ArrowVersion2_C'"));
	if (tempArrowVer2Factory.Succeeded()) {
		ArrowVer2Factory = tempArrowVer2Factory.Class;
	}

	static ConstructorHelpers::FClassFinder<AGrenade> tempGrenadeFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_Grenade.BP_Grenade_C'"));
	if (tempGrenadeFactory.Succeeded()) {
		GrenadeFactory = tempGrenadeFactory.Class;
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(105.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	projectileComp = CreateDefaultSubobject<USceneComponent>(TEXT("projectileComp"));
	projectileComp->SetupAttachment(RootComponent);
	projectileComp->SetRelativeLocation(FVector(24, 56, 132));

	projectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("projectilePath"));
	projectilePath->SetupAttachment(RootComponent);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeLocation(FVector(0.854105, 0, -106.501848));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/Sova/TPS/sovaComplex.sovaComplex'"));
	if (tempSK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}

	ConstructorHelpers::FClassFinder<USB_SovaAnim> tempAnimTPS(TEXT("/Script/Engine.AnimBlueprint'/Game/SB/Blueprints/ABP_SovaTPS.ABP_SovaTPS_C'"));
	if (tempAnimTPS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimTPS.Class);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempFpsMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/Sova/FPS/RealSovaFPS/sova1st.sova1st'"));
	if (tempFpsMesh.Succeeded())
	{
		fpsMesh->SetSkeletalMesh(tempFpsMesh.Object);
	}
	fpsMesh->SetRelativeLocation(FVector(0, 0, -106.5));
	fpsMesh->SetRelativeRotation(FRotator(0, -90, 0));
	fpsMesh->CastShadow = false;

	static ConstructorHelpers::FClassFinder<USB_SovaAnim> tempSovaFpsAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/SB/Blueprints/ABP_SovaFPS.ABP_SovaFPS_C'"));
	if (tempSovaFpsAnim.Succeeded()) {
		fpsMesh->SetAnimInstanceClass(tempSovaFpsAnim.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> tempBaseWeaponFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_BaseWeapon.BP_BaseWeapon_C'"));
	if (tempBaseWeaponFactory.Succeeded()) {
		BaseWeaponFactory = tempBaseWeaponFactory.Class;
	}

	BaseWeapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("BaseWeapon"));
	BaseWeapon->SetupAttachment(fpsMesh, TEXT("WeaponLoc"));
	BaseWeapon->SetRelativeLocation(FVector(1.700801, -9.536563, 0.029019));
	BaseWeapon->SetRelativeRotation(FRotator(0, -11, 208));
	BaseWeapon->SetRelativeScale3D(FVector(1.17));
	BaseWeapon->SetChildActorClass(BaseWeaponFactory);

	arrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("arrowObj"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempArrowMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/BowAndArrow/Bow_and_arrow.Bow_and_arrow'"));
	if (tempArrowMesh.Succeeded()) {
		arrowMesh->SetSkeletalMesh(tempArrowMesh.Object);
	}
	arrowMesh->SetupAttachment(fpsMesh, TEXT("ArrowSocket"));
	arrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	arrowMesh->SetRelativeScale3D(FVector(0.01));
	arrowMesh->SetVisibility(false);
	arrowMesh->SetRelativeLocation(FVector(0.321571, -0.005168, 0.002690));
	arrowMesh->SetRelativeRotation(FRotator(0, -75, 0));
	arrowMesh->CastShadow = false;

	ArrowFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowFirePos"));
	ArrowFirePos->SetupAttachment(RootComponent);
	ArrowFirePos->SetRelativeLocation(FVector(64, 30, 34));

	static ConstructorHelpers::FObjectFinder<USoundBase> tempScoutingArrowVoice1(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/ScoutingArrow/sova_ScoutingArrow0.sova_ScoutingArrow0'"));
	if (tempScoutingArrowVoice1.Succeeded()) {
		ScoutingArrowVoice1 = tempScoutingArrowVoice1.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> tempScoutingArrowVoice2(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/ScoutingArrow/sova_ScoutingArrow1.sova_ScoutingArrow1'"));
	if (tempScoutingArrowVoice2.Succeeded()) {
		ScoutingArrowVoice2 = tempScoutingArrowVoice2.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> tempScoutingArrowVoice3(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/ScoutingArrow/sova_ScoutingArrow2.sova_ScoutingArrow2'"));
	if (tempScoutingArrowVoice3.Succeeded()) {
		ScoutingArrowVoice3 = tempScoutingArrowVoice3.Object;
	}

	static ConstructorHelpers::FClassFinder<USkillWidget> tempSovaSkillUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_Sova_Skill.WB_Sova_Skill_C'"));
	if (tempSovaSkillUI.Succeeded()) {
		SkillWidgetFactory = tempSovaSkillUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> tempSmokeSkillUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_SmokeSkillUI.WB_SmokeSkillUI_C'"));
	if (tempSmokeSkillUI.Succeeded()) {
		smokeSkillUIFactory = tempSmokeSkillUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> tempProtoSmokeSkillUIFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_Proto_SmokeSkillUI.WB_Proto_SmokeSkillUI_C'"));
	if (tempSmokeSkillUI.Succeeded()) {
		ProtoSmokeSkillUIFactory = tempProtoSmokeSkillUIFactory.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor> tempSmokeObjFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_SpawnSmokeObj.BP_SpawnSmokeObj_C'"));
	if (tempSmokeObjFactory.Succeeded()) {
		SmokeObjFactory = tempSmokeObjFactory.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> tempSovaAirSmokeVoice1(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/AirSmoke/sova_AirSmoke0.sova_AirSmoke0'"));
	if (tempSovaAirSmokeVoice1.Succeeded()) {
		SovaAirSmokeVoice1 = tempSovaAirSmokeVoice1.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> tempSovaAirSmokeVoice2(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/AirSmoke/sova_AirSmoke1.sova_AirSmoke1'"));
	if (tempSovaAirSmokeVoice2.Succeeded()) {
		SovaAirSmokeVoice2 = tempSovaAirSmokeVoice2.Object;
	}
	ConstructorHelpers::FClassFinder<USB_AirSmokeMarker> tempAirSmokeMarkerFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/UMG/WB_AirSmokeMarker.WB_AirSmokeMarker_C'"));
	if (tempAirSmokeMarkerFactory.Succeeded()) {
		AirSmokeMarkerFactory = tempAirSmokeMarkerFactory.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempSovaGrenadeMongtage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animations/Grenade/AN_SovaGrenade.AN_SovaGrenade'"));
	if (tempSovaGrenadeMongtage.Succeeded()) {
		SovaGrenadeMongtage = tempSovaGrenadeMongtage.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> tempGrenadeSound1(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/Grenade/sova_Grenade0.sova_Grenade0'"));
	if (tempGrenadeSound1.Succeeded()) {
		GrenadeSound1 = tempGrenadeSound1.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> tempGrenadeSound2(TEXT("/Script/Engine.SoundWave'/Game/SB/Sounds/Grenade/sova_Grenade1.sova_Grenade1'"));
	if (tempGrenadeSound2.Succeeded()) {
		GrenadeSound2 = tempGrenadeSound2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> tempRopeMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Rope/Rope_M_Inst.Rope_M_Inst'"));
	if (tempRopeMat.Succeeded())
	{
		RopeMat = tempRopeMat.Object;
	}

	CableComp = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	CableComp->SetupAttachment(fpsMesh, TEXT("L_Shoulder"));
	CableComp->SetRelativeScale3D(FVector(0.01));
	CableComp->CableLength = 10;
	CableComp->NumSegments = 1;
	CableComp->CableWidth = 7.5;
	CableComp->NumSides = 16;
	CableComp->SetAttachEndTo(this, TEXT("HookColl"));
	CableComp->SetMaterial(0, RopeMat);
	CableComp->CableGravityScale = 0;
	CableComp->CastShadow = false;

	static ConstructorHelpers::FClassFinder<AActor> tempDeathCameraFactory(TEXT("/Script/Engine.Blueprint'/Game/LMH/BP/BP_DeathCamera.BP_DeathCamera_C'"));
	if (tempDeathCameraFactory.Succeeded()) {
		DeathCameraFactory = tempDeathCameraFactory.Class;
	}

	DeathCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("DeathCamera"));
	DeathCamera->SetupAttachment(RootComponent);
	DeathCamera->SetChildActorClass(DeathCameraFactory);
	DeathCamera->SetRelativeLocation(FVector(-187, 0, 121));
	DeathCamera->SetRelativeRotation(FRotator(-70, 0, 0));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempDieMontage(TEXT("/Script/Engine.AnimMontage'/Game/LMH/Animation/Standing_React_Death_Backward1_Montage.Standing_React_Death_Backward1_Montage'"));
	if (tempDieMontage.Succeeded()) {
		DieMontage = tempDieMontage.Object;
	}

	CameraBoom->SetRelativeLocation(FVector(-164, 0, 65));
	CameraBoom->TargetArmLength = 0;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	static ConstructorHelpers::FClassFinder<ASB_Hook> tempHookActorFactory(TEXT("/Script/Engine.Blueprint'/Game/SB/Blueprints/BP_HookActor.BP_HookActor_C'"));
	if (tempHookActorFactory.Succeeded()) {
		HookActorFactory = tempHookActorFactory.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> tempDefaultMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SB/Materials/M_CubeBeam_Inst.M_CubeBeam_Inst'"));
	if (tempDefaultMaterial.Succeeded())
	{
		GrenadeSplineMat = tempDefaultMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempGrenadeSplineMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Models/splinebeam.splinebeam'"));
	if (tempGrenadeSplineMesh.Succeeded()) {
		GrenadeSplineMesh = tempGrenadeSplineMesh.Object;
	}
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = 1;
	bReplicates = true;
}

void ASB_Sova::BeginPlay()
{
	Super::BeginPlay();

	soundKill = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/2_throw.2_throw'"));
	expl = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/5_expl.5_expl'"));

	FAttachmentTransformRules AttachCamRule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	CameraBoom->AttachToComponent(fpsMesh, AttachCamRule, TEXT("Head"));

	if (IsLocallyControlled()) {
		GetMesh()->SetVisibility(false);
		fpsMesh->SetVisibility(true);
	}
	else {
		GetMesh()->SetVisibility(true);
		fpsMesh->SetVisibility(false);
	}
}

void ASB_Sova::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bThrowHook && MyHook != nullptr) {
		FVector MoveDir = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), MyHook->GetActorLocation());
		GetCharacterMovement()->AddForce(MoveDir * GrappleMovePower);
		float ToHookDist = (GetActorLocation() - MyHook->GetActorLocation()).Length();
		if (ToHookDist < GrappleMoveStopDist && MyHook->bThrowHook == false) {
			EndGrappleMove();
		}
	}

	if (IsLocallyControlled() == false) return;
	AirSmokeMouseCursor();

	ToGrappleDeltaTime = DeltaTime;

	if (mouseLeftClick) {
		switch (currState)
		{
		case ESovaState::DefaultAtk:
			break;
		case ESovaState::ScoutingArrow:
			ArrowPowerGaugeUp();
			break;
		case ESovaState::AirSmoke:
			break;
		case ESovaState::Grenade:
			break;
		default:
			break;
		}
	}
	// ASB_Sova::Tick 매 프레임마다 궤적을 생성함
	if (bThrowing) ShowProjectilePath();
}

void ASB_Sova::MouseLeftAction()
{
	if (IsLocallyControlled() == false) return;
	mouseLeftClick = true;
	switch (currState)
	{
	case ESovaState::DefaultAtk:
		if (fireComp && !fireComp->bReloadOn) {
			if (!bSniperOn) {
				if (HasAuthority()) {
					ServerDefaultShootPress_Implementation();
				}
				else {
					ServerDefaultShootPress();
				}
				//DefaultShootPress();
			}
			else {
				fireComp->SniperShot();
			}
		}
		break;
	case ESovaState::ScoutingArrow:
		break;
	case ESovaState::AirSmoke:
		break;
	case ESovaState::Grenade:
		if (HasAuthority()) {
			ServerGrenadeThrowAction_Implementation();
		}
		else {
			ServerGrenadeThrowAction();
		}
		break;
	default:
		break;
	}
}

void ASB_Sova::MouseLeftReleasedAction()
{
	if (IsLocallyControlled() == false) return;
	mouseLeftClick = false;

	switch (currState)
	{
	case ESovaState::DefaultAtk:
		if (HasAuthority()) {
			fireComp->ServerStopFire_Implementation();
		}
		else {
			fireComp->ServerStopFire();
		}
		//if (!bSniperOn) {
			//DefaultShootRelease();
		//}
		//else {
		//	// 스나이퍼 모드
		//}
		break;
	case ESovaState::ScoutingArrow:
		if (HasAuthority()) {
			Server_SetBoolScoutingArrow_Implementation(false);
		}
		else {
			Server_SetBoolScoutingArrow(false);
		}
		gaugeCurrTime = 0;
		if (ui_SB_ScoutingArrowInstance) {
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(0);
		}
		ScoutingArrowShot();
		ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Hidden);
		InitScoutingArrow();
		currState = ESovaState::DefaultAtk;
		break;
	case ESovaState::AirSmoke:
		break;
	case ESovaState::Grenade:
		break;
	default:
		break;
	}
}

void ASB_Sova::MouseRightAction()
{
	if (IsLocallyControlled() == false) return;
	switch (currState)
	{
	case ESovaState::DefaultAtk:
		bSniperOn = true;
		if (skillWigetInstance) {
			skillWigetInstance->AimingPanelOn();
			BaseWeapon->SetHiddenInGame(true);
			fpsMesh->SetHiddenInGame(true);
		}
		break;
	case ESovaState::ScoutingArrow:
		IncreaseBounceCount();
		break;
	case ESovaState::AirSmoke:
		AirSmokeLogic();
		break;
	case ESovaState::Grenade:
		break;
	default:
		break;
	}
}

void ASB_Sova::MouseRightReleasedAction()
{
	switch (currState)
	{
	case ESovaState::DefaultAtk:
		bSniperOn = false;
		if (skillWigetInstance) {
			skillWigetInstance->AimingPanelOff();
			BaseWeapon->SetHiddenInGame(false);
			fpsMesh->SetHiddenInGame(false);
		}
		break;
	case ESovaState::ScoutingArrow:
		break;
	case ESovaState::AirSmoke:
		break;
	case ESovaState::Grenade:
		break;
	default:
		break;
	}
}

void ASB_Sova::KeyE()
{
	if (IsLocallyControlled() == false) return;
	if (currState == ESovaState::DefaultAtk) {
		currState = ESovaState::ScoutingArrow;
		if (HasAuthority()) {
			Server_SetBoolScoutingArrow_Implementation(true);
		}
		else {
			Server_SetBoolScoutingArrow(true);
		}
		ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Visible);
	}
	else if (currState == ESovaState::ScoutingArrow) {
		currState = ESovaState::DefaultAtk;
		if (HasAuthority()) {
			Server_SetBoolScoutingArrow_Implementation(false);
		}
		else {
			Server_SetBoolScoutingArrow(false);
		}
		ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Hidden);
		InitScoutingArrow();
	}
}

void ASB_Sova::KeyQ()
{
	if (IsLocallyControlled() == false) return;
	if (currState == ESovaState::DefaultAtk) {
		currState = ESovaState::Grenade;
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		if (HasAuthority()) {
			Server_SetCurrState_Implementation(currState);
			ServerGrenadeThrowReady_Implementation(MyController);
		}
		else {
			Server_SetCurrState(currState);
			ServerGrenadeThrowReady(MyController);
		}
	}
	else if (currState == ESovaState::Grenade) {
		currState = ESovaState::DefaultAtk;
		if (HasAuthority()) {
			Server_SetCurrState_Implementation(currState);
			ServerCancelGrenade_Implementation();
		}
		else {
			Server_SetCurrState(currState);
			ServerCancelGrenade();
		}
		ClearPath();
		bThrowing = false;
	}
}

void ASB_Sova::KeyC()
{
	if (IsLocallyControlled() == false) return;
	if (currState == ESovaState::DefaultAtk) {
		currState = ESovaState::AirSmoke;
		ActiveAirSmoke();
	}
	else if (currState == ESovaState::AirSmoke) {
		currState = ESovaState::DefaultAtk;
		DeactiveAirSmoke();
	}
}

void ASB_Sova::KeyF()
{
	if (IsLocallyControlled() == false) return;
	if(bCanGrappleAction == false) return;
	GrappleAction();
}

void ASB_Sova::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> PosseedBy"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	if (IsLocallyControlled()) {
		GetMesh()->SetVisibility(false);
		fpsMesh->SetVisibility(true);
	}
	else {
		GetMesh()->SetVisibility(true);
		fpsMesh->SetVisibility(false);
	}
}

void ASB_Sova::PlayDieMontage()
{
	if (IsLocallyControlled()) {
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(DeathCamera->GetChildActor(), 1, EViewTargetBlendFunction::VTBlend_EaseInOut, 1);
		GetMesh()->SetVisibility(true);
	}

	GetMesh()->GetAnimInstance()->Montage_Play(DieMontage);
	fpsMesh->GetAnimInstance()->Montage_Play(DieMontage);
}

void ASB_Sova::MeshVisible()
{
	if (FollowCamera) {
		FollowCamera->Activate();
	}
	if (DeathCamera) {
		DeathCamera->Deactivate();
	}
	GetMesh()->SetVisibility(false);
}

void ASB_Sova::Server_SetCurrState_Implementation(ESovaState paramCurrState)
{
	Multicast_SetCurrState(paramCurrState);
}

void ASB_Sova::Multicast_SetCurrState_Implementation(ESovaState paramCurrState)
{
	currState = paramCurrState;
}

void ASB_Sova::Server_SetBoolScoutingArrow_Implementation(bool bScoutingChk)
{
	Mulitcast_SetBoolScoutingArrow(bScoutingChk);
}

void ASB_Sova::Mulitcast_SetBoolScoutingArrow_Implementation(bool bScoutingChk)
{ // 스킬 활성화 상태가 아니라면 화살을 숨김
	bScoutingArrow = bScoutingChk;
	if (bScoutingChk) {
		if (!arrowMesh->GetVisibleFlag()) {
			isGun = false;
			arrowMesh->SetVisibility(true);
			BaseWeapon->SetVisibility(false);
		}
	}
	else {
		if (arrowMesh->GetVisibleFlag()) {
			isGun = true;
			arrowMesh->SetVisibility(false);
			BaseWeapon->SetVisibility(true);
		}
	}
}

void ASB_Sova::InitScoutingArrow()
{
	skillBounceCount = 0;
	ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
	ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
}

void ASB_Sova::ScoutingArrowShot()
{
	if (auto PlayerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)) {
		FVector TraceStartLoc = PlayerCam->GetCameraLocation();
		FVector TraceEndLoc = TraceStartLoc + (PlayerCam->GetActorForwardVector() * 5000);

		FHitResult HitResult;
		FCollisionQueryParams param;
		param.AddIgnoredActor(GetOwner()); // 라인트레이스가 물체와 충돌하면 충돌한 위치에 액터 스폰
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLoc, TraceEndLoc, ECC_GameTraceChannel4, param);

		if (HitResult.bBlockingHit) {
			TraceEndLoc = HitResult.ImpactPoint;
		}

		FVector ArrowSpawnLoc = ArrowFirePos->GetComponentLocation();
		FVector ArrowDir = TraceEndLoc - ArrowSpawnLoc;
		FRotator ArrowSpawnRotator = ArrowDir.GetSafeNormal().Rotation();
		FVector InDirVec = ArrowDir.GetSafeNormal();
		if (auto MyController = GetWorld()->GetFirstPlayerController()) {
			FTransform ArrowTransform = FTransform(ArrowSpawnRotator.Quaternion(), ArrowSpawnLoc, FVector(1));
			if (HasAuthority()) {
				Server_SpawnArrow_Implementation(MyController, ArrowTransform, skillBounceCount, InDirVec, scoutingArrowSpeed);
			}
			else {
				Server_SpawnArrow(MyController, ArrowTransform, skillBounceCount, InDirVec, scoutingArrowSpeed);
			}
			int RanVal = UKismetMathLibrary::RandomIntegerInRange(0, 2);
			switch (RanVal)
			{
			case 0:
				UGameplayStatics::PlaySound2D(GetWorld(), ScoutingArrowVoice1);
				break;
			case 1:
				UGameplayStatics::PlaySound2D(GetWorld(), ScoutingArrowVoice2);
				break;
			case 2:
				UGameplayStatics::PlaySound2D(GetWorld(), ScoutingArrowVoice3);
				break;
			default:
				break;
			}
		}
	}
}

void ASB_Sova::Server_SpawnArrow_Implementation(APlayerController* MyPlayer, FTransform transform, int32 bounceCount, FVector InDirVec, float ArrowSpeed)
{
	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnConfig.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnConfig.Owner = MyPlayer;
	auto doFunc = [ArrowSpeed, InDirVec](AActor* ObjectToModify)
		{
			ASB_ArrowVersion2* arrowToModify = Cast<ASB_ArrowVersion2>(ObjectToModify);
			if (arrowToModify)
			{
				arrowToModify->InitSpeed = ArrowSpeed;
				arrowToModify->InitDirVector = InDirVec;
			}
		};

	spawnConfig.CustomPreSpawnInitalization = doFunc;

	ASB_ArrowVersion2* arrow = GetWorld()->SpawnActor<ASB_ArrowVersion2>(ArrowVer2Factory, transform.GetLocation(), FRotator::MakeFromEuler(transform.GetRotation().Euler()), spawnConfig);
	if (arrow) {
		arrow->maxBounceCount = bounceCount;
	}
}
// 화살 속도 조절
void ASB_Sova::ArrowPowerGaugeUp()
{
	gaugeCurrTime += GetWorld()->GetDeltaSeconds();
	if (gaugeCurrTime < gaugeTime) {
		float alpha = gaugeCurrTime / gaugeTime;
		scoutingArrowSpeed = FMath::Lerp(minScoutingArrowSpeed, maxScoutingArrowSpeed, alpha);
		if (ui_SB_ScoutingArrowInstance) {
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(alpha);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetFillColorAndOpacity(ui_SB_ScoutingArrowInstance->LackGaugeColor);
		}
	}
	else {
		if (ui_SB_ScoutingArrowInstance) {
			scoutingArrowSpeed = FMath::Lerp(minScoutingArrowSpeed, maxScoutingArrowSpeed, 1.0f);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(1);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetFillColorAndOpacity(ui_SB_ScoutingArrowInstance->FullGaugeColor);
		}
	}
}
// 화살 튕김 횟수 증가
void ASB_Sova::IncreaseBounceCount()
{
	if (ui_SB_ScoutingArrowInstance == nullptr) return;

	skillBounceCount++;
	if (skillBounceCount > 2) skillBounceCount = 0; // 최대횟수 2회가 초과하면 다시 0으로 초기화
	// 화살 튕김횟수 UI로 표시
	if (skillBounceCount == 0) { // XX
		ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
		ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
	}
	else if (skillBounceCount == 1) { // XO
		ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
		ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
	}
	else if (skillBounceCount == 2) { // OO
		ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
		ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
	}
}

void ASB_Sova::ServerGrenadeThrowReady_Implementation(APlayerController* MyPlayerController) {
	MulticastGrenadeThrowReady(MyPlayerController);
}

void ASB_Sova::MulticastGrenadeThrowReady_Implementation(APlayerController* MyPlayerController)
{
	auto FpsAnim = fpsMesh->GetAnimInstance();
	FpsAnim->Montage_Play(SovaGrenadeMongtage, 1.0f);
	auto TpsAnim = GetMesh()->GetAnimInstance();
	TpsAnim->Montage_Play(SovaGrenadeMongtage, 1.0f);

	if (HasAuthority()) {
		ServerSpawnGrenade_Implementation(MyPlayerController);
	}
}

void ASB_Sova::ShowProjectilePath()
{
	if (bThrowing) {
		// 이전 프레임에서 생성한 궤적 제거
		ClearPath();

		FVector StartPos = projectileComp->GetComponentLocation();
		FVector ThrowVelocity = GetThrowVelocity();
		FHitResult HitResult;
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDest;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		// 수류탄을 던지는 속력에 따른 예측 궤적의 위치값들을 배열에 저장
		UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, OutPathPositions, OutLastTraceDest, StartPos, ThrowVelocity, true, 0, ECC_Camera, false, ActorsToIgnore, EDrawDebugTrace::None, 0, 15, 3, 0);

		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			projectilePath->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Local, true);
		}

		//스플라인의 선분(Segment) 갯수에서 스플라인 메쉬의 겹침 문제를 없애기 위해 1을 빼줌
		int32 LastIndex = projectilePath->GetNumberOfSplineSegments() - 1;
		for (int j = 0; j < LastIndex; j++)
		{
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			//실린더 메쉬를 사용할떄 곡면이 있는 부분이 X축 정면일때, 평평한 윗면 Z축을 정면으로 변경
			SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
			SplineMeshComponent->SetStaticMesh(GrenadeSplineMesh);
			// 메쉬 움직일 수 있도록 설정
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			//월드에 등록
			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
			//궤적 시작 위치부터 스플라인 메쉬를 추가
			SplineMeshComponent->AttachToComponent(projectilePath, FAttachmentTransformRules::KeepWorldTransform);
			//메쉬 스케일 조절
			SplineMeshComponent->SetStartScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(0.1f)));
			SplineMeshComponent->SetEndScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(0.1f)));

			//시작과 끝지점의 위치값과 탄젠트값을 구하여 부드러운 곡선을 만듦
			const FVector StartPoint = projectilePath->GetLocationAtSplinePoint(j, ESplineCoordinateSpace::Local);
			const FVector StartTangent = projectilePath->GetTangentAtSplinePoint(j, ESplineCoordinateSpace::Local);
			const FVector EndPoint = projectilePath->GetLocationAtSplinePoint(j + 1, ESplineCoordinateSpace::Local);
			const FVector EndTangent = projectilePath->GetTangentAtSplinePoint(j + 1, ESplineCoordinateSpace::Local);
			SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (GrenadeSplineMat)
			{ // 머테리얼 할당
				SplineMeshComponent->SetMaterial(0, GrenadeSplineMat);
			}
			// 궤적 메쉬를 매프레임마다 생성하기 때문에 
			// 이전 프레임에서 생성한 궤적을 지우기 위해 메쉬 컴포넌트 포인터 배열 저장
			SplineMeshComponents.Add(SplineMeshComponent);
		}
	}
}

FVector ASB_Sova::GetThrowVelocity()
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector ThrowVelocity = (ForwardVector + FVector(0, 0, 0.4)) * ThrowPower;
	return ThrowVelocity;
}

void ASB_Sova::ClearPath() {
	projectilePath->ClearSplinePoints(true);
	if (SplineMeshComponents.Num() > 0)
	{
		for (int32 i = 0; i < SplineMeshComponents.Num(); i++)
		{
			if (SplineMeshComponents[i])
			{
				SplineMeshComponents[i]->DestroyComponent();
			}
		}
		SplineMeshComponents.Empty();
	}
}

void ASB_Sova::ServerCancelGrenade_Implementation()
{
	if (MyGrenade) {
		MyGrenade->Destroy();
	}
}

void ASB_Sova::ServerSpawnGrenade_Implementation(APlayerController* MyPlayerController)
{
	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASB_Sova* MySova = Cast<ASB_Sova>(MyPlayerController->GetPawn());
	spawnConfig.Owner = MySova;

	MyGrenade = GetWorld()->SpawnActor<AGrenade>(GrenadeFactory, MySova->fpsMesh->GetSocketLocation(TEXT("Grenade")), FRotator(0), spawnConfig);
}

void ASB_Sova::ServerGrenadeThrowAction_Implementation()
{
	if (bThrowing) {
		MulticastGrenadeThrowAction();
		bThrowing = false;
		if (MyGrenade) {
			if (HasAuthority()) {
				FTimerHandle DelayHandle;
				GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]()->void {
					MyGrenade->Throw(GetThrowVelocity());
					}), 0.1f, false);
			}
		}
	}
	else {
		if (MyGrenade) {
			MyGrenade->Destroy();
		}
	}
}

void ASB_Sova::MulticastGrenadeThrowAction_Implementation()
{
	auto FpsAnim = fpsMesh->GetAnimInstance();
	FpsAnim->Montage_Play(SovaGrenadeMongtage, 1.0f);
	FpsAnim->Montage_JumpToSection(TEXT("End"), SovaGrenadeMongtage);

	auto TpsAnim = GetMesh()->GetAnimInstance();
	TpsAnim->Montage_Play(SovaGrenadeMongtage, 1.0f);
	TpsAnim->Montage_JumpToSection(TEXT("End"), SovaGrenadeMongtage);

	int RanVal = UKismetMathLibrary::RandomIntegerInRange(0, 1);
	switch (RanVal)
	{
	case 0:
		UGameplayStatics::PlaySound2D(GetWorld(), GrenadeSound1);
		break;
	case 1:
		UGameplayStatics::PlaySound2D(GetWorld(), GrenadeSound2);
		break;
	}
	if (IsLocallyControlled()) {
		ClearPath();
		bThrowing = false;
	}
}

void ASB_Sova::ActiveAirSmoke()
{
	GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> ActiveAirSmoke"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
	if (smokeSkillUIinstance) {
		smokeSkillUIinstance->SetVisibility(ESlateVisibility::Visible);
		if (auto Pc = GetWorld()->GetFirstPlayerController()) {
			Pc->SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Pc, smokeSkillUIinstance);
			PlayerRef = Pc;
			AirSmokeMarkerRef = CreateWidget<USB_AirSmokeMarker>(GetWorld(), AirSmokeMarkerFactory);
			if (AirSmokeMarkerRef) {
				AirSmokeMarkerRef->AddToViewport();
			}
		}
	}
}
void ASB_Sova::DeactiveAirSmoke()
{
	if (smokeSkillUIinstance) {
		smokeSkillUIinstance->SetVisibility(ESlateVisibility::Hidden);
		if (auto PC = GetWorld()->GetFirstPlayerController()) {
			PC->SetShowMouseCursor(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
			if (AirSmokeMarkerRef) {
				AirSmokeMarkerRef->SetVisibility(ESlateVisibility::Hidden);
			}
			airSmokeCurrCount = 0;
			OnRemoveSmokerUI.Broadcast();
			if (auto SmokeSkillUI = Cast<UAirSmokeMinimapWidget>(smokeSkillUIinstance)) {
				SmokeSkillUI->SpawnSmokePos.Empty();
				currState = ESovaState::DefaultAtk;
			}
		}
	}
}

void ASB_Sova::AirSmokeLogic()
{
	AirSmokeVoice(); // 소바 스킬 시전 효과음 : "시야를 차단해"
	if (auto SmokeSkillUI = Cast<UAirSmokeMinimapWidget>(smokeSkillUIinstance)) {
		for (auto& SpawnSmokePosVal : SmokeSkillUI->SpawnSmokePos)
		{
			FVector StartLoc = FVector(SpawnSmokePosVal.X, SpawnSmokePosVal.Y, 5000);
			FVector EndLoc = FVector(SpawnSmokePosVal.X, SpawnSmokePosVal.Y, -5000);
			FHitResult HitResult;
			FCollisionQueryParams param;
			param.AddIgnoredActor(GetOwner()); // 라인트레이스가 물체와 충돌하면 충돌한 위치에 액터 스폰
			bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_GameTraceChannel7, param);
			if (IsHit) { // 서버 RPC함수에 스폰할 위치를 인자값으로 넘겨주고, 서버에서 공중연막 액터 스폰
				if (HasAuthority()) {
					ServerSpawnSmokeObj_Implementation(HitResult.ImpactPoint);
				}
				else {
					ServerSpawnSmokeObj(HitResult.ImpactPoint);
				}
			}
		}
		SmokeSkillUI->SpawnSmokePos.Empty();
		DeactiveAirSmoke();
	}
}

void ASB_Sova::AirSmokeVoice()
{
	if (airSmokeCurrCount > 0) {
		int32 RanVal = UKismetMathLibrary::RandomIntegerInRange(0, 1);
		switch (RanVal)
		{
		case 0:
			UGameplayStatics::PlaySound2D(GetWorld(), SovaAirSmokeVoice1);
			break;
		case 1:
			UGameplayStatics::PlaySound2D(GetWorld(), SovaAirSmokeVoice2);
			break;
		default:
			break;
		}
	}
}

void ASB_Sova::ServerSpawnSmokeObj_Implementation(FVector SpawnPos)
{
	FRotator Rot;
	GetWorld()->SpawnActor<AActor>(SmokeObjFactory, SpawnPos, Rot);
}

void ASB_Sova::AirSmokeMouseCursor()
{
	if (AirSmokeMarkerRef) {
		FVector2D MakeVec = AirSmokeMarkerRef->GetDesiredSize() * UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / FVector2D(1920, 1080);
		FVector2D MakeVec2 = FVector2D(MakeVec.Y, MakeVec.Y) / FVector2D(2, 2);
		float LocX, LocY;
		if (PlayerRef != nullptr && PlayerRef->GetMousePosition(LocX, LocY)) {
			FVector2D MarkerPos = FVector2D(LocX, LocY) - MakeVec2;
			AirSmokeMarkerRef->SetPositionInViewport(MarkerPos);
		}
	}
}

void ASB_Sova::NotifyRestarted()
{
	Super::NotifyRestarted();

	if (IsLocallyControlled()) {
		if (ui_SB_ScoutingArrowFactory) {
			ui_SB_ScoutingArrowInstance = CreateWidget<UUI_SB_ScoutingArrow>(GetWorld(), ui_SB_ScoutingArrowFactory);
			ui_SB_ScoutingArrowInstance->AddToViewport();
			ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SkillWidgetFactory) {
			skillWigetInstance = CreateWidget<USkillWidget>(GetWorld(), SkillWidgetFactory);
			skillWigetInstance->AddToViewport();
			skillWigetInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (bProtoMapSkillMode == false) {
			if (smokeSkillUIFactory) {
				smokeSkillUIinstance = CreateWidget<UUserWidget>(GetWorld(), smokeSkillUIFactory);
				smokeSkillUIinstance->AddToViewport();
				smokeSkillUIinstance->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else {
			if (ProtoSmokeSkillUIFactory) {
				smokeSkillUIinstance = CreateWidget<UUserWidget>(GetWorld(), ProtoSmokeSkillUIFactory);
				smokeSkillUIinstance->AddToViewport();
				smokeSkillUIinstance->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void ASB_Sova::HookNotify()
{
	if (HasAuthority()) {
		LaunchCharacter(FVector(0, 0, 500), true, true);
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]() {
			bThrowHook = true;
		}), 0.2f, false);

		FTimerHandle EndHookSkillHandle;
		GetWorld()->GetTimerManager().SetTimer(EndHookSkillHandle, FTimerDelegate::CreateLambda([&]() {
			if (bCanGrappleAction == false && MyHook && MyHook->bThrowHook == false) {
				EndGrappleMove();
			}
			}), 2.5f, false);
	}
}

void ASB_Sova::ServerGrappleAction_Implementation()
{
	bCanGrappleAction = false;
	FVector StartLoc = GetActorLocation();
	GrappleEndLoc = StartLoc + GetFollowCamera()->GetForwardVector() * GrappleHookDistance;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	ActorsToIgnore.Add(this);
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, GrappleEndLoc, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Blue, 0.1f);

	FVector HookStartPos = fpsMesh->GetSocketLocation(TEXT("L_Shoulder"));
	FRotator HookRot = UKismetMathLibrary::FindLookAtRotation(HookStartPos, GrappleEndLoc);

	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnConfig.Owner = this;

	MyHook = GetWorld()->SpawnActor<ASB_Hook>(HookActorFactory, fpsMesh->GetSocketLocation(TEXT("L_Shoulder")), HookRot, spawnConfig);

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]() {
		if (bCanGrappleAction == false && MyHook->bThrowHook == true) {
			EndGrappleMove();
			}
		}), 2.0f, false);
}

void ASB_Sova::AttachHook(class ASB_Hook* HookPtr)
{
	if (HookPtr) {
		CableComp->SetVisibility(true);
		CableComp->SetAttachEndToComponent(HookPtr->HookMesh);
		CableComp->AttachEndToSocketName = TEXT("EndHook");
	}
}

void ASB_Sova::MulticastEndGrappleAction_Implementation()
{
	CableComp->SetVisibility(false);
}

void ASB_Sova::EndGrappleMove()
{
	if (MyHook && HasAuthority()) {
		bThrowHook = false;
		GetCharacterMovement()->Velocity = FVector(0);
		if (MyHook->bThrowHook == false) {
			LaunchCharacter(FVector(0, 0, 900), true, true);
		}
		bCanGrappleAction = MyHook->Destroy();
	}
	MulticastEndGrappleAction();
}

void ASB_Sova::GrappleAction()
{
	if (HasAuthority()) {
		ServerGrappleAction_Implementation();
	}
	else {
		ServerGrappleAction();
	}
}

void ASB_Sova::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_Sova, bScoutingArrow);
	DOREPLIFETIME(ASB_Sova, scoutingArrowSpeed);
	DOREPLIFETIME(ASB_Sova, isGun);
	DOREPLIFETIME(ASB_Sova, currState);
	DOREPLIFETIME(ASB_Sova, airSmokeCurrCount);
	DOREPLIFETIME(ASB_Sova, bThrowing);
	DOREPLIFETIME(ASB_Sova, bCanGrappleAction);
}

