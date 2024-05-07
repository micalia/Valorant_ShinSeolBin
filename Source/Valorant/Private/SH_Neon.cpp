// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_Neon.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "PSH_Weapon.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "SH_HitTestPlayer.h"
#include "SH_Neon_AnimInstance.h"
#include "Components/Viewport.h"
#include "Math/UnrealMathUtility.h"
#include "FireUserWidget.h"
#include "Components/SizeBox.h"
#include "Engine/DecalActor.h"
#include "PSH_Neon_RelayBolt.h"
#include "PSH_Neon_FastLane.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "PSH_SkillWidget.h"
#include "Net/UnrealNetwork.h"
#include "RespawnWidget.h"
#include "Components/PostProcessComponent.h"
#include "SH_FastLane_OverlapActor.h"
#include "Nasunhwan.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Particles/ParticleSystemComponent.h>


ASH_Neon::ASH_Neon() {
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/PSH/New_Neon/neon1st.neon1st'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -85));
	}

	//T_meshcomp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("3_Player"));

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetRelativeScale3D(FVector(1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempT_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/PSH/Resouce/Neon/bvgfv7667.bvgfv7667'"));
	if (TempT_Mesh.Succeeded())
	{
		fpsMesh->SetSkeletalMesh(TempT_Mesh.Object);
		fpsMesh->SetRelativeLocation(FVector(0, 0, -85));
		fpsMesh->SetRelativeRotation(FRotator(0, -90, 0));
	}
	fpsMesh->SetupAttachment(RootComponent);
	// 	T_meshcomp->bOnlyOwnerSee = true;
	// 	T_meshcomp->bOwnerNoSee = false;


	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SetRelativeLocation(FVector(30, 0, 110));


	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);


	// 
	// 	GetMesh()->bOnlyOwnerSee = false;
	// 	GetMesh()->bOwnerNoSee = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;

	post = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessSK"));
	post->SetupAttachment(FollowCamera);


	//mh
	if (TempStaticMesh.Succeeded())
	{
		Sphere->SetupAttachment(FollowCamera);
		Sphere->SetRelativeLocation(FVector(400, 0, 0));
		Sphere->SetStaticMesh(TempStaticMesh.Object);
		Sphere->SetCollisionProfileName(TEXT("Winston"));
		Sphere->SetGenerateOverlapEvents(true);
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Sphere->SetCollisionObjectType(ECC_GameTraceChannel2);
		Sphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		Sphere->SetVisibility(false);
	}

	CM = CreateDefaultSubobject<UCameraComponent>(TEXT("CM"));
	CM->SetupAttachment(Sphere);
	CM->bUsePawnControlRotation = false; // Camera does not rotate relative to arm 
	CM->bAutoActivate = false;
	CM->SetRelativeLocation(FVector(-400, 0, 80));
	CM->SetRelativeRotation(FRotator(-10, 0, 0));

	NSHLeftPos = CreateDefaultSubobject<USceneComponent>(TEXT("NSHLeftPos"));
	NSHLeftPos->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	NSHRightPos = CreateDefaultSubobject<USceneComponent>(TEXT("NSHRightPos"));
	NSHRightPos->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	//bReplicates=true;
}

void ASH_Neon::BeginPlay()
{
	Super::BeginPlay();

	post->SetVisibility(false);

	anim = Cast<USH_Neon_AnimInstance>(GetMesh()->GetAnimInstance());
	//T_anim = Cast<USH_Neon_AnimInstance>(GetMesh()->GetAnimInstance());
	
	//Respawn_UI = CreateWidget<URespawnWidget>(GetWorld(), RespawnWidget);
	//if (Respawn_UI != nullptr)
	//{
	//	Respawn_UI->AddToViewport();
	//	////GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("RespawnUI!!!!!")), true, FVector2D(1, 1));
	//	//버튼 입력 받을 수 있게??
	//}
}

void ASH_Neon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isHighGear) // e스킬 사용중
	{
		energy -= 6 * DeltaTime;
		if (energy <= 0)
		{
			energy = 0;
			GetCharacterMovement()->MaxWalkSpeed = curSpeed;
			ischarge = true;
			isHighGear = false;
		}
	}

	if (ischarge)
	{
		energy += 2 * DeltaTime;
		if (energy >= 100)
		{
			energy = 100;
			ischarge = false;
		}
	}
	// 	if (isFire)
	// 	{
	// 		StartFire(DeltaTime);
	// 	}
}


void ASH_Neon::MouseLeftAction()
{
	DefaultShootPress();
	//anim->PlayFireAnimation();
	//isFire = true;
}

void ASH_Neon::MouseLeftReleasedAction()
{
	DefaultShootRelease();
	//StopFire();
}

void ASH_Neon::OnHitWall(const FHitResult& hitResult)
{
	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/PSH/Resouce/Bullethole/MT_BulletHole.MT_BulletHole'"));

	if (DecalMaterial)
	{
		// 데칼 액터 생성
		FTransform DecalTransform = FTransform(hitResult.ImpactNormal.Rotation(), hitResult.ImpactPoint);
		ADecalActor* DecalActor = GetWorld()->SpawnActor<ADecalActor>(DecalTransform.GetLocation(), DecalTransform.GetRotation().Rotator());

		if (DecalActor)
		{
			// 데칼 크기와 회전 설정
			FVector DecalScale = FVector(10.0f, 10.0f, 10.0f); // 데칼 크기 조절
			DecalActor->SetActorScale3D(DecalScale);

			// 데칼 액터의 머티리얼 설정
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, DecalActor);
			DecalActor->SetDecalMaterial(DynamicMaterial);

			// 데칼 액터 초기화
			DecalActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DecalActor->RegisterAllComponents();
		}
	}
}

void ASH_Neon::Skill_Q_Anim_Implementation()
{
	anim->Play_1P_Skill_Q_Animation();
}

void ASH_Neon::Skill_C_Anim_Implementation()
{
	anim->Play_1P_Skill_C_Animation();
}

void ASH_Neon::Fire_Anim_Implementation()
{
	anim->Play_3P_Fire_Animation();
}

void ASH_Neon::NotifyRestarted()
{
	Super::NotifyRestarted();

	fire_UI = CreateWidget<UFireUserWidget>(GetWorld(), fireWidget);
	if (fire_UI != nullptr)
	{
		fire_UI->AddToViewport();
	}
	Skill_UI = CreateWidget<UPSH_SkillWidget>(GetWorld(), SkillWidget);
	if (Skill_UI != nullptr)
	{
		Skill_UI->AddToViewport();
	}
}

void ASH_Neon::MouseRightAction()
{
	if (isHighGear)
	{
		//애니메이션 플레이
	}
	//g 무기 버리기 로 나중에 변환
	if (owningWeapon != nullptr)
	{
		owningWeapon->ReleaseWeapon(this);
	}
}



void ASH_Neon::KeyQ()
{
	/*MultiSpawnVolt();*/
	SeverSpawnVolt();
	Skill_Q_Anim();
	/*BP_Play_1P_Skill_Q_Animation();*/
}

void ASH_Neon::KeyE()
{
	if (!isHighGear)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			gearSound,
			GetActorLocation()
		);
		GetCharacterMovement()->MaxWalkSpeed = UpSpeed;
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SkillE,
			GetActorLocation()
		);
		//Skill_UI->EnergyVisible();
		isHighGear = true;
		ischarge = false;
		HasWeapon = false;
		post->SetVisibility(true);
		// 렾유항ㄴ;ㅓㅎㅇㄴ;헝니
	}
	else
	{
		FollowCamera->SetPostProcessBlendWeight(0.f);
		GetCharacterMovement()->MaxWalkSpeed = curSpeed;
		isHighGear = false;
		ischarge = true;
		HasWeapon = true;
		//Skill_UI->EnergyHiden();
		post->SetVisibility(false);
	}
	//UE_LOG(LogTemp, Warning, TEXT("GearOn"));
}

void ASH_Neon::KeyX()
{
	if (magazine > 0)
	{
		magazine--;
		ammo = 30;
	}
}

void ASH_Neon::KeyC()
{
	Skill_C_Anim();
	/*BP_Play_3P_Skill_C_Animation();*/
	SuverSpawnLane();
}

void ASH_Neon::WeaponInfoReset()
{
	ammo = 0;
	fireInterval = 0;
	attackPower = 0;
}

void ASH_Neon::AttachWeapon(TSubclassOf<class APSH_Weapon> weapon)
{
	if (weapon)
	{
		// SpawnActor를 통해 weapon 데이터를 기반으로 한 액터 생성
		_equipWeapon = GetWorld()->SpawnActor<APSH_Weapon>(weapon);

		// 소켓 이름을 통해 현재 메시에서 소켓을 참조
		const USkeletalMeshSocket* weaponSocket = GetMesh()->GetSocketByName("WeaponSocket");

		if (_equipWeapon && weaponSocket)
		{
			// 소켓에 액터를 할당
			weaponSocket->AttachActor(_equipWeapon, GetMesh());
		}
	}
}

void ASH_Neon::Reload()
{
	owningWeapon->Reloading(this);
}

void ASH_Neon::HitProcess()
{
	// 	APlayerController* pc = GetController<APlayerController>();
	// 	if (pc != nullptr)
	// 	{
	// 	pc->ClientStartCameraShake(hitShake);
	// 	}

}

void ASH_Neon::ReleaseWeapon() // 나중에 g버튼이랑 바인드
{
	if (owningWeapon != nullptr)
	{
		owningWeapon->ReleaseWeapon(this);
	}
}

void ASH_Neon::SuverSpawnLane_Implementation()
{
	Multisound();
	FVector startPos = FollowCamera->GetComponentLocation(); // 발사 위치
		/*FVector ForwardDirection = GetActorForwardVector(); //발사 방향*/
	FRotator Rotation = Controller->GetControlRotation();

	FActorSpawnParameters parm;
	parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<APSH_Neon_FastLane>(fastLaneFat, startPos, Rotation, parm);
	// 추월 차선

	FVector leftDiagonaVector(-1.0f, -1.0f, -1.0f);
	lefStartLoc = startPos + leftDiagonaVector * 200;

	//Right StartPos
	FVector rightDiagonaVector(-1.0f, 1.0f, -1.0f);
	rightStartLoc = startPos + rightDiagonaVector * 200;
	

	L_WallPos = lefStartLoc;
	R_WallPos = rightStartLoc;
	GetWorld()->SpawnActor<ASH_FastLane_OverlapActor>(LaneOverlaFat, lefStartLoc, Rotation, parm);
	GetWorld()->SpawnActor<ASH_FastLane_OverlapActor>(LaneOverlaFat, rightStartLoc, Rotation, parm);
}

void ASH_Neon::Multisound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		SkillC,
		GetActorLocation()
	);
	UGameplayStatics::PlaySoundAtLocation(
		this,
		FastLaneSound,
		GetActorLocation()
	);
}


void ASH_Neon::SeverSpawnVolt_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		VoltSound,
		GetActorLocation()
	);
	FVector startPos = FollowCamera->GetComponentLocation(); // 발사 위치
	FRotator Rotation = Controller->GetControlRotation();
	if (SkillQ)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SkillQ,
			GetActorLocation()
		);
	}
	FActorSpawnParameters parm;
	parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<APSH_Neon_RelayBolt>(relayVolt_BulletFat, startPos, Rotation, parm);
}

void ASH_Neon::MultiSpawnVolt_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("nonSever"));
}

