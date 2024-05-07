// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_Sova.h"
#include "UI_SB_ScoutingArrow.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/ProgressBar.h>
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
#include "SH_Neon.h"
#include <UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Sound/SoundBase.h>

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
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(105.0f);

	projectileComp = CreateDefaultSubobject<USceneComponent>(TEXT("projectileComp"));
	projectileComp->SetupAttachment(RootComponent);
	projectileComp->SetRelativeLocation(FVector(42.765113, 54.495774, 64.635261));

	projectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("projectilePath"));
	projectilePath->SetupAttachment(RootComponent);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeLocation(FVector(0.854105, 0, -106.501848));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Script/Engine.SkeletalMesh'/Game/SB/Models/Sova/TPS/sovaComplex.sovaComplex'"));
	if (tempSK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}


	static ConstructorHelpers::FClassFinder<USB_SovaAnim> tempAnimTPS(TEXT("/Script/Engine.AnimBlueprint'/Game/SB/Blueprints/ABP_SovaTPS.ABP_SovaTPS_C'"));
	if (tempAnimTPS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimTPS.Class);
	}

	/*fpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("fpsMesh"));
	fpsMesh->SetupAttachment(GetCapsuleComponent());*/
	
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

	bReplicates = true;
}

void ASB_Sova::BeginPlay()
{
	Super::BeginPlay();

	soundKill = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/2_throw.2_throw'"));
	expl = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/5_expl.5_expl'"));

	NGGuideLineEm = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/SB/Effect/Nebul/NS_Torus.NS_Torus"));
}

void ASB_Sova::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled() == false) return;
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
		case ESovaState::MinhaTeleport:
			break;
		default:
			break;
		}
	}

	if (currState == ESovaState::MinhaTeleport)
	{
		FVector start = FollowCamera->GetComponentLocation();
		FVector end = FollowCamera->GetComponentLocation() + ((FollowCamera->GetForwardVector()) * MaxDistance);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params);

		if (bHit)
		{
			Impacted = true;
			if (Impacted)
			{
				ImpactPoint = HitResult.ImpactPoint;
				if (NGGuideLine) {
					NGGuideLine->SetWorldLocation(ImpactPoint);
				}
			}
			//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 2.0f);
		}
	}
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
		GrenadeThrowAction();
		break;
	case ESovaState::MinhaTeleport: {
		if (Impacted)
		{
			if (NGGuideLine)
			{
				FVector pos = NGGuideLine->GetComponentLocation();
				pos.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				if (HasAuthority()) {
					Server_SetLocation_Implementation(pos);
				}
				else {
					Server_SetLocation(pos);
				}

				NGGuideLine->DestroyComponent();

				Impacted = false;
				currState = ESovaState::DefaultAtk;
			}
		}
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
		//bScoutingArrow = false;
		gaugeCurrTime = 0;
		if (ui_SB_ScoutingArrowInstance) {
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(0);
		}
		Fire();
		ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Hidden);
		InitScoutingArrow();
		currState = ESovaState::DefaultAtk;
		break;
	case ESovaState::AirSmoke:
		break;
	case ESovaState::Grenade:
		break;
	case ESovaState::MinhaTeleport:
		break;
	default:
		break;
	}
	
}

void ASB_Sova::MouseRightAction()
{
	if(IsLocallyControlled() == false) return;
	switch (currState)
	{
	case ESovaState::DefaultAtk:
		break;
	case ESovaState::ScoutingArrow:
		IncreaseBounceCount();
		break;
	case ESovaState::AirSmoke:
		break;
	case ESovaState::Grenade:
		break;
	case ESovaState::MinhaTeleport:
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
		////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("E")), true, FVector2D(1, 1));
		if (HasAuthority()) {
			Server_SetBoolScoutingArrow_Implementation(true);
		}
		else {
			Server_SetBoolScoutingArrow(true);
		}
		//bScoutingArrow = true;
		//SetScoutingArrow(true);
		ui_SB_ScoutingArrowInstance->SetVisibility(ESlateVisibility::Visible);
	}
	else if(currState == ESovaState::ScoutingArrow){
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
		if (HasAuthority()) {
			Server_SetCurrState_Implementation(currState);
		}
		else {
			Server_SetCurrState(currState);
		}
		GrenadeThrowReady();
	}
	else if(currState == ESovaState::Grenade){
		currState = ESovaState::DefaultAtk;
		if (HasAuthority()) {
			Server_SetCurrState_Implementation(currState);
		}
		else {
			Server_SetCurrState(currState);
		}
		ClearPath();
		CancelGrenade();
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
	//if (currState == ESovaState::DefaultAtk) {
	//	currState = ESovaState::MinhaTeleport;
	//	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCcast0,GetActorLocation());
	//	GuideLine = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GuideLineEm, ImpactPoint);
	//}
	//else if (currState == ESovaState::MinhaTeleport) {
	//	currState = ESovaState::DefaultAtk;
	//	if (GuideLine) {
	//		GuideLine->DestroyComponent();
	//	}
	//}
	if (currState == ESovaState::DefaultAtk) {
		currState = ESovaState::MinhaTeleport;
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCcast0,GetActorLocation());

		if (NGGuideLineEm)
		{
			NGGuideLine = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NGGuideLineEm, ImpactPoint);
		}
		else {
			/*DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 2.0f);*/
			////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("11111111111111111111111")), true, FVector2D(1, 1));
		}
		
	}
	else if (currState == ESovaState::MinhaTeleport) {
		currState = ESovaState::DefaultAtk;
		if (NGGuideLine) {
			NGGuideLine->DestroyComponent();
		}
	}
}

void ASB_Sova::KeyV()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, FString::Printf(TEXT("v")), true, FVector2D(1, 1));
}

void ASB_Sova::KeyG()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, FString::Printf(TEXT("g")), true, FVector2D(1, 1));
}

void ASB_Sova::KeyB()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, FString::Printf(TEXT("b")), true, FVector2D(1, 1));
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
{
	bScoutingArrow = bScoutingChk;
	//if (HasAuthority()) {
	if (bScoutingChk) {
		if (!arrowMesh->GetVisibleFlag()) {
			isGun = false;
			arrowMesh->SetVisibility(true);
		}
	}
	else {
		if(arrowMesh->GetVisibleFlag()) {
			isGun = true;
			arrowMesh->SetVisibility(false);
		}
	}
	//}
}

void ASB_Sova::InitScoutingArrow()
{
	skillBounceCount = 0;
	ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
	ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
}

void ASB_Sova::Server_SpawnArrow_Implementation(FTransform transform, int32 bounceCount)
{
	MultiCast_SpawnArrow_Implementation(transform, bounceCount);
}

void ASB_Sova::MultiCast_SpawnArrow_Implementation(FTransform transform, int32 bounceCount)
{
	FActorSpawnParameters spawnConfig;
	spawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnConfig.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	float customSpeed = scoutingArrowSpeed;
	auto doFunc = [customSpeed](AActor* ObjectToModify)
	{
		ASB_Arrow* arrowToModify = Cast<ASB_Arrow>(ObjectToModify);
		if (arrowToModify)
		{
			arrowToModify->initSpeed = customSpeed;
		}
	};

	spawnConfig.CustomPreSpawnInitalization= doFunc;
		
	ASB_Arrow* arrow = GetWorld()->SpawnActor<ASB_Arrow>(arrowFactory, transform.GetLocation(), FRotator::MakeFromEuler(transform.GetRotation().Euler()), spawnConfig);
	if (arrow) {
		arrow->SetOwner(this);
		arrow->maxBounceCount = bounceCount;
	}
}

void ASB_Sova::ArrowPowerGaugeUp()
{
	gaugeCurrTime+=GetWorld()->GetDeltaSeconds();
	if (gaugeCurrTime < gaugeTime) {
		float alpha = gaugeCurrTime / gaugeTime;
		if (HasAuthority()) {
			Server_SetFloatScoutingArrow_Implementation(alpha);
		}
		else {
			Server_SetFloatScoutingArrow(alpha);
		}
		//scoutingArrowSpeed = FMath::Lerp(minScoutingArrowSpeed, maxScoutingArrowSpeed, alpha);
		if (ui_SB_ScoutingArrowInstance) {
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(alpha);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetFillColorAndOpacity(ui_SB_ScoutingArrowInstance->LackGaugeColor);
		}
	}
	else {
		if (ui_SB_ScoutingArrowInstance) {
			if (HasAuthority()) {
				Server_SetFloatScoutingArrow_Implementation(1);
			}
			else {
				Server_SetFloatScoutingArrow(1);
			}
			//scoutingArrowSpeed = FMath::Lerp(minScoutingArrowSpeed, maxScoutingArrowSpeed, 1.0f);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetPercent(1);
			ui_SB_ScoutingArrowInstance->PowerGaugeBar->SetFillColorAndOpacity(ui_SB_ScoutingArrowInstance->FullGaugeColor);
		}
	}
}

void ASB_Sova::Server_SetFloatScoutingArrow_Implementation(float arrowSpeedAlpha)
{
	Mulitcast_SetFloatScoutingArrow_Implementation(arrowSpeedAlpha);
}

void ASB_Sova::Mulitcast_SetFloatScoutingArrow_Implementation(float arrowSpeedAlpha)
{
	scoutingArrowSpeed = FMath::Lerp(minScoutingArrowSpeed, maxScoutingArrowSpeed, arrowSpeedAlpha);
}

void ASB_Sova::IncreaseBounceCount()
{
	if(ui_SB_ScoutingArrowInstance == nullptr) return;
	if(IsLocallyControlled() == false) return;
	
		skillBounceCount++;

		if (skillBounceCount == 0) {
			ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
			ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
		}
		else if (skillBounceCount == 1) {
			ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
			ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
		}
		else if (skillBounceCount == 2) {
			ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
			ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->ActiveColor);
		}
		else if (skillBounceCount > 2)
		{
			skillBounceCount = 0;
			ui_SB_ScoutingArrowInstance->BounceCount1_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
			ui_SB_ScoutingArrowInstance->BounceCount2_img->SetColorAndOpacity(ui_SB_ScoutingArrowInstance->NotActiveColor);
		}

}

void ASB_Sova::Server_SetLocation_Implementation(FVector pos)
{
	Multicast_SetLocation_Implementation(pos);
}

void ASB_Sova::Multicast_SetLocation_Implementation(FVector pos)
{
	SetActorLocation(pos);
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
				/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("widget ")), true, FVector2D(1, 1));*/
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
}

