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

	projectileComp = CreateDefaultSubobject<USceneComponent>(TEXT("projectileComp"));
	projectileComp->SetupAttachment(RootComponent);
	projectileComp->SetRelativeLocation(FVector(24, 56, 132));

	projectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("projectilePath"));
	projectilePath->SetupAttachment(RootComponent);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeLocation(FVector(0.854105, 0, -106.501848));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

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

	arrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("arrowObj"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempArrowMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/BowAndArrow/Bow_and_arrow.Bow_and_arrow'"));
	if (tempArrowMesh.Succeeded()) {
		arrowMesh->SetSkeletalMesh(tempArrowMesh.Object);
	}
	arrowMesh->SetupAttachment(fpsMesh, TEXT("ArrowSocket"));
	arrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	static ConstructorHelpers::FClassFinder<UFireUserWidget> tempFireWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WBP_Fire.WBP_Fire_C'"));
	if (tempFireWidget.Succeeded()) {
		fireWidget = tempFireWidget.Class;
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

	bReplicates = true;
}

void ASB_Sova::BeginPlay()
{
	Super::BeginPlay();

	soundKill = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/2_throw.2_throw'"));
	expl = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/5_expl.5_expl'"));
}

void ASB_Sova::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled() == false) return;
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
// ASB_Sova::Tick �� �����Ӹ��� ������ ������
	if (bThrowing) ShowProjectilePath();
}

void ASB_Sova::MouseLeftAction()
{
	if (IsLocallyControlled() == false) return;
	mouseLeftClick = true;
	switch (currState)
	{
	case ESovaState::DefaultAtk:
		DefaultShootPress();
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
		DefaultShootRelease();
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
	Mulitcast_SetBoolScoutingArrow_Implementation(bScoutingChk);
}

void ASB_Sova::Mulitcast_SetBoolScoutingArrow_Implementation(bool bScoutingChk)
{ // ��ų Ȱ��ȭ ���°� �ƴ϶�� ȭ���� ����
	bScoutingArrow = bScoutingChk;
	if (bScoutingChk) {
		if (!arrowMesh->GetVisibleFlag()) {
			isGun = false;
			arrowMesh->SetVisibility(true);
		}
	}
	else {
		if (arrowMesh->GetVisibleFlag()) {
			isGun = true;
			arrowMesh->SetVisibility(false);
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
		param.AddIgnoredActor(GetOwner()); // ����Ʈ���̽��� ��ü�� �浹�ϸ� �浹�� ��ġ�� ���� ����
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
// ȭ�� �ӵ� ����
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
// ȭ�� ƨ�� Ƚ�� ����
void ASB_Sova::IncreaseBounceCount()
{
	if (ui_SB_ScoutingArrowInstance == nullptr) return;

	skillBounceCount++;
	if (skillBounceCount > 2) skillBounceCount = 0; // �ִ�Ƚ�� 2ȸ�� �ʰ��ϸ� �ٽ� 0���� �ʱ�ȭ
	// ȭ�� ƨ��Ƚ�� UI�� ǥ��
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
		// ���� �����ӿ��� ������ ���� ����
		ClearPath();

		FVector StartPos = projectileComp->GetComponentLocation();
		FVector ThrowVelocity = GetThrowVelocity();
		FHitResult HitResult;
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDest;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		// ����ź�� ������ �ӷ¿� ���� ���� ������ ��ġ������ �迭�� ����
		UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, OutPathPositions, OutLastTraceDest, StartPos, ThrowVelocity, true, 0, ECC_Camera, false, ActorsToIgnore, EDrawDebugTrace::None, 0, 15, 3, 0);

		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			projectilePath->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::Local, true);
		}

		//���ö����� ����(Segment) �������� ���ö��� �޽��� ��ħ ������ ���ֱ� ���� 1�� ����
		int32 LastIndex = projectilePath->GetNumberOfSplineSegments() - 1;
		for (int j = 0; j < LastIndex; j++)
		{
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			//�Ǹ��� �޽��� ����ҋ� ����� �ִ� �κ��� X�� �����϶�, ������ ���� Z���� �������� ����
			SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
			SplineMeshComponent->SetStaticMesh(DefalutMesh);
			// �޽� ������ �� �ֵ��� ����
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			//���忡 ���
			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
			//���� ���� ��ġ���� ���ö��� �޽��� �߰�
			SplineMeshComponent->AttachToComponent(projectilePath, FAttachmentTransformRules::KeepWorldTransform);
			//�޽� ������ ����
			SplineMeshComponent->SetStartScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(0.1f)));
			SplineMeshComponent->SetEndScale(FVector2D(UKismetSystemLibrary::MakeLiteralFloat(0.1f), UKismetSystemLibrary::MakeLiteralFloat(0.1f)));

			//���۰� �������� ��ġ���� ź��Ʈ���� ���Ͽ� �ε巯�� ��� ����
			const FVector StartPoint = projectilePath->GetLocationAtSplinePoint(j, ESplineCoordinateSpace::Local);
			const FVector StartTangent = projectilePath->GetTangentAtSplinePoint(j, ESplineCoordinateSpace::Local);
			const FVector EndPoint = projectilePath->GetLocationAtSplinePoint(j + 1, ESplineCoordinateSpace::Local);
			const FVector EndTangent = projectilePath->GetTangentAtSplinePoint(j + 1, ESplineCoordinateSpace::Local);
			SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (DefaultMaterial)
			{ // ���׸��� �Ҵ�
				SplineMeshComponent->SetMaterial(0, DefaultMaterial);
			}
			// ���� �޽��� �������Ӹ��� �����ϱ� ������ 
			// ���� �����ӿ��� ������ ������ ����� ���� �޽� ������Ʈ ������ �迭 ����
			SplineMeshComponents.Add(SplineMeshComponent);
		}
	}
}

FVector ASB_Sova::GetThrowVelocity()
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector ThrowVelocity = (ForwardVector + FVector(0, 0, 0.4)) * 800;
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
	AirSmokeVoice(); // �ҹ� ��ų ���� ȿ���� : "�þ߸� ������"
	if (auto SmokeSkillUI = Cast<UAirSmokeMinimapWidget>(smokeSkillUIinstance)) {
		for (auto& SpawnSmokePosVal : SmokeSkillUI->SpawnSmokePos)
		{
			FVector StartLoc = FVector(SpawnSmokePosVal.X, SpawnSmokePosVal.Y, 5000);
			FVector EndLoc = FVector(SpawnSmokePosVal.X, SpawnSmokePosVal.Y, -5000);
			FHitResult HitResult;
			FCollisionQueryParams param;
			param.AddIgnoredActor(GetOwner()); // ����Ʈ���̽��� ��ü�� �浹�ϸ� �浹�� ��ġ�� ���� ����
			bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_GameTraceChannel7, param);
			if (IsHit) { // ���� RPC�Լ��� ������ ��ġ�� ���ڰ����� �Ѱ��ְ�, �������� ���߿��� ���� ����
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

		if (fireWidget)
		{
			fire_UI = CreateWidget<UFireUserWidget>(GetWorld(), fireWidget);
			if (fire_UI != nullptr)
			{
				fire_UI->AddToViewport();
			}
		}
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
}

