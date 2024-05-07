// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Omen.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"	
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include <Particles/ParticleSystemComponent.h>
#include "Math/Vector.h"
#include <Kismet/KismetSystemLibrary.h>
#include <DrawDebugHelpers.h>
#include "Net/UnrealNetwork.h"
#include "Left_NSH.h"
#include "Q_NSH.h"
#include <Kismet/KismetMathLibrary.h>
#include <Delegates/Delegate.h>
#include "Nasunhwan.h"
#include "PlayerFireComponent.h"


AMH_Omen::AMH_Omen() {
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

	if (TempMesh.Succeeded())
	{ 
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -89));
		GetMesh()->SetRelativeRotation(FRotator(0, -89, 0));
	}

	Sphere=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (TempStaticMesh.Succeeded())
	{
		Sphere->SetupAttachment(FollowCamera);
		Sphere->SetRelativeLocation(FVector(400, 0, 0));
		Sphere->SetStaticMesh(TempStaticMesh.Object);
		Sphere->SetCollisionProfileName(TEXT("Winston"));
		Sphere->SetGenerateOverlapEvents(true);
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Sphere->SetCollisionObjectType(ECC_GameTraceChannel2);
		Sphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
		Sphere->SetVisibility(false);
	}

	CM = CreateDefaultSubobject<UCameraComponent>(TEXT("CM"));
	CM->SetupAttachment(Sphere);
	CM->bUsePawnControlRotation = false; // Camera does not rotate relative to arm 
	CM->bAutoActivate=false;
	CM->SetRelativeLocation(FVector(-400,0,80));
	CM->SetRelativeRotation(FRotator(-10,0,0));

	NSHLeftPos = CreateDefaultSubobject<USceneComponent>(TEXT("NSHLeftPos"));
	NSHLeftPos->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	NSHRightPos = CreateDefaultSubobject<USceneComponent>(TEXT("NSHRightPos"));
	NSHRightPos->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	//ConstructorHelpers::FClassFinder<UMainWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LMH/BP/WB_MainHud.WB_MainHud'"));

	//if (TempWidget.Succeeded())
	//{
	//	main_UI->set
	//}

	PlayFire = CreateDefaultSubobject<UPlayerFireComponent>(TEXT("PlayFire"));

	bReplicates=true;
}

//void AMH_Omen::SwitchMode(ESkill_EMode Mode)
//{
//	if (Mode==ESkill_EMode::ModeB)
//	{
//		ESkill_Enum=ESkill_EMode::ModeA;
//		CameraModeLevel();
//	}
//	else {
//		ESkill_Enum = ESkill_EMode::ModeB;
//		PlayerModeLevel();
//	}
//}

//void AMH_Omen::CameraSwitching()
//{
//	if (ESkill_Enum == ESkill_EMode::ModeA && distCameraToSphere() >= 400 && FollowCamera->IsActive())
//	{
//		CameraToCM();
//	}
//	else if (distCameraToSphere() < 400 && CM->IsActive())
//	{
//		CameraToFC();
//	}
//}

//void AMH_Omen::CameraToFC()
//{
//	CM->Deactivate();
//	FollowCamera->SetActive(true);
//}
//
//void AMH_Omen::CameraToCM()
//{
//	FollowCamera->Deactivate();
//	CM->SetActive(true);
//}

//FVector AMH_Omen::CameraPos()
//{
//	return (FollowCamera->GetForwardVector()) * CameraToMarbleDistance +( FollowCamera->GetComponentLocation());
//}
//
//float AMH_Omen::Forwarding()
//{
//	return CameraToMarbleDistance + ((GetWorld()->DeltaTimeSeconds)* Speed);
//}
//
//float AMH_Omen::Backwarding()
//{
//	return CameraToMarbleDistance+ ((GetWorld()->DeltaTimeSeconds) * (Speed)*(-1));
//}
//
//float AMH_Omen::distCameraToSphere()
//{
//	float dist=FVector::Distance(Sphere->GetComponentLocation(), FollowCamera->GetComponentLocation());
//
//	return dist;
//}

//void AMH_Omen::Active()
//{
//	//play sound
//	FActorSpawnParameters spawnParams;
//	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	FVector dir= FollowCamera->GetForwardVector();
//	dir.Normalize(0.0001);
//	//FRotator rotator = UKismetMathLibrary::MakeRotFromX(dir.Normalize());
//	FRotator rotator = UKismetMathLibrary::MakeRotFromX(dir);
//	
//	FVector spawnLocation = FollowCamera->GetComponentLocation();
//
//	NSH = GetWorld()->SpawnActor<AQ_NSH>(AQ_NSH::StaticClass(), spawnLocation, rotator, spawnParams);
//
//	if (NSH)
//	{
//		/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("nsh!!!!")), true, FVector2D(1, 1));*/
//		AActor* nsh = Cast<AActor>(NSH);
//		NSH->OnDestroyed.AddDynamic(this, &AMH_Omen::MyDestroyed);
//		//nsh->OnDestroyed.AddDynamic(this, &AMH_Omen::MyDestroyed);
//	}
//}

//void AMH_Omen::MyDestroyed(AActor* nshParam)
//{
//	IsQ = false;
//	IsE=false;
//	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("destroyed!")), true, FVector2D(1, 1));*/
//}

void AMH_Omen::BeginPlay()
{
	Super::BeginPlay();
	//ESkill_Enum=ESkill_EMode::ModeA;
	/*GuideLineEm =LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));*/

	SoundCcast0= LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/Play_VO_Wraith_AbilityCCast_0.Play_VO_Wraith_AbilityCCast_0'"));
	SoundCcast1= LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/Play_VO_Wraith_AbilityCCast_1.Play_VO_Wraith_AbilityCCast_1'"));
	SoundEcast2 = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/LMH/Sounds/Play_VO_Wraith_AbilityECast_2.Play_VO_Wraith_AbilityECast_2'"));
	// 메인 위젯 생성하기
	//if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	//{
	//	battle_UI = CreateWidget<UBattleWidget>(GetWorld(), battleWidget);
	//	if (battle_UI != nullptr)
	//	{
	//		battle_UI->AddToViewport();
	//	}
	//}
		// 메인 위젯 생성하기
	if (GetController() != nullptr)
	{
		main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
		if (main_UI != nullptr)
		{
			main_UI->AddToViewport();
		}
	}
	myLocalRole = GetLocalRole();
	myRemoteRole = GetRemoteRole();
}

void AMH_Omen::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);
	timeTest += DeltaSeconds;
	PrintLog();
	//if (IsC)
	//{
	//	CameraSwitching();
	//	FVector start = FollowCamera->GetComponentLocation();
	//	FVector end = FollowCamera->GetComponentLocation() + ((FollowCamera->GetForwardVector()) * MaxDistance);

	//	FHitResult HitResult;
	//	FCollisionQueryParams Params;
	//	Params.AddIgnoredActor(this);

	//	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params);

	//	if (bHit)
	//	{
	//		Impacted = true;
	//		if (Impacted)
	//		{
	//			ImpactPoint = HitResult.ImpactPoint;
	//			GuideLine->SetWorldLocation(ImpactPoint);
	//		}
	//		//DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 2.0f);
	//	}
	//}
	//if (IsE)
	//{
	//	CameraSwitching();
	//	if (distCameraToSphere() <= 200)
	//		IsBlocking = true;

	//	if (IsForward)
	//	{
	//		IsBlocking = false;
	//		CameraToMarbleDistance = Forwarding();
	//		Sphere->SetWorldLocation(CameraPos());
	//	}
	//	if (IsBackward)
	//	{
	//		if (!IsBlocking)
	//		{
	//			CameraToMarbleDistance = Backwarding();
	//			Sphere->SetWorldLocation(CameraPos());
	//		}
	//	}
	//}
}

//void AMH_Omen::KeyQ()
//{
//	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("KeyQ")), true, FVector2D(1, 1));*/
//	if (Qcnt >= 1)
//	{
//		IsQ = true;
//
//		FActorSpawnParameters spawnParams;
//		spawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		FRotator rotator=FRotator::ZeroRotator;
//		FVector spawnLocation = NSHLeftPos->GetComponentLocation();
//
//		SpawnedNSH =GetWorld()->SpawnActor<ALeft_NSH>(ALeft_NSH::StaticClass(), spawnLocation, rotator, spawnParams);
//
//		if (SpawnedNSH)
//		{
//			FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
//
//			SpawnedNSH->AttachToComponent(NSHLeftPos,AttachmentRules);
//			/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("spawn actor")), true, FVector2D(1, 1));*/
//		}
//	}
//
//}

//void AMH_Omen::KeyC()
//{
//	if (!IsC)
//	{
//		IsC=true;
//		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCcast0,GetActorLocation());
//		GuideLine=UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GuideLineEm,ImpactPoint);
//	}
//}

//void AMH_Omen::KeyE()
//{
//	if (!IsE)
//	{
//		IsE=true;
//		Sphere->SetVisibility(true);
//		if (ESkill_Enum==ESkill_EMode::ModeB)
//		{
//			SwitchMode(ESkill_Enum);
//		}
//	}
//	else {
//		if (SpawnedENSH==nullptr)
//		{
//			if (ESkill_Enum==ESkill_EMode::ModeA)
//			{
//				SwitchMode(ESkill_Enum);
//				CameraToFC();
//			}
//			Sphere->SetVisibility(false);
//
//			FActorSpawnParameters spawnParams;
//			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//			FVector dir=(Sphere->GetComponentLocation()- NSHLeftPos->GetComponentLocation());
//			dir.Normalize(0.0001);
//
//			FRotator rotator = UKismetMathLibrary::MakeRotFromX(dir);
//			FVector spawnLocation = NSHLeftPos->GetComponentLocation();
//
//
//			SpawnedENSH = GetWorld()->SpawnActor<ANasunhwan>(SpawnedNSHFactory, spawnLocation, rotator, spawnParams);
//
//			if (SpawnedENSH)
//			{
//				SpawnedENSH->TargetPos=Sphere->GetComponentLocation();
//					/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("nsh!!!!")), true, FVector2D(1, 1));*/
//				AActor* nsh = Cast<AActor>(SpawnedENSH);
//				nsh->OnDestroyed.AddDynamic(this, &AMH_Omen::MyDestroyed);
//			}
//		}
//	}
//}

//void AMH_Omen::MouseLeftAction()
//{
//	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("LeftClick")), true, FVector2D(1, 1));*/
//	IsForward=true;
//	if (Impacted && IsC)
//	{
//		FVector pos= GuideLine->GetComponentLocation();
//		SetActorLocation(pos);
//		GuideLine->DestroyComponent();
//
//		IsC=false;
//		Impacted=false;
//	}
//
//	if (IsFire)
//	{
//		DefaultShootPress();
//	}
//	else {
//		if (IsQ && Qcnt>=1)
//		{
//			Qcnt--;
//			SpawnedNSH->Destroy();
//			IsQ=false;
//			Active();
//		}
//	}
//}


//void AMH_Omen::MouseLeftReleasedAction()
//{
//	IsForward = false;
//
//	if (IsFire)
//	{
//		DefaultShootRelease();
//	}
//}

void AMH_Omen::PrintLog()
{
	const FString localRoleString = UEnum::GetValueAsString<ENetRole>(myLocalRole);
	const FString remoteRoleString = UEnum::GetValueAsString<ENetRole>(myRemoteRole);
	const FString ownerString = GetOwner() != nullptr ? GetOwner()->GetName() : FString("No Owner");
	const FString connectionString = GetNetConnection() != nullptr ? FString("Valid Connection") : FString("Invalid Connection");
	const FString printString = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nOwner Name: %s\nNet Connection : %s"), *localRoleString, *remoteRoleString, *ownerString, *connectionString);

	DrawDebugString(GetWorld(), GetActorLocation(), printString, nullptr, FColor::White, 0, true);
}

//void AMH_Omen::MouseRightReleasedAction()
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("RightReleased")), true, FVector2D(1, 1));
//}

//void AMH_Omen::KeyQ()
//{
//	Super::KeyQ();
//	if (Qcnt>=1)
//	{	
//		IsQ=true;
//		SpawnNSHEm=UGameplayStatics::SpawnEmitterAttached(NSHEmitter,NSHPos);
//	}
//}
void AMH_Omen::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AMH_Omen, timeTest); //Cond settings 주의
	DOREPLIFETIME(AMH_Omen, timeTest);
	//DOREPLIFETIME(ANetworkTestCharacter, jumpCount);
	//DOREPLIFETIME(ANetworkTestCharacter, owningWeapon);
	//DOREPLIFETIME(ANetworkTestCharacter, ammo);
	//DOREPLIFETIME(ANetworkTestCharacter, attackPower);
	//DOREPLIFETIME(ANetworkTestCharacter, fireInterval);
	//DOREPLIFETIME(ANetworkTestCharacter, health);
}
