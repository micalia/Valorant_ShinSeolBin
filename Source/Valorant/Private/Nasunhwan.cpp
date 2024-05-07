// Fill out your copyright notice in the Description page of Project Settings.


#include "Nasunhwan.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <Components/SphereComponent.h>
#include <Math/Vector.h>
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>

// Sets default values
ANasunhwan::ANasunhwan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->SetSphereRadius(500);
	SphereCollision->SetRelativeScale3D(FVector(0.5));
	//SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->SetCollisionProfileName(TEXT("Winston"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	smoke = CreateDefaultSubobject<UNiagaraComponent>(TEXT("smokeMH"));
	smoke->SetupAttachment(Sphere);


	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	//UNiagaraSystem* NS = LoadObject<UNiagaraSystem>(nullptr, TEXT(""), nullptr, LOAD_None, nullptr);

	if (TempStaticMesh.Succeeded())
	{
		Sphere->SetStaticMesh(TempStaticMesh.Object);
		Sphere->SetupAttachment(RootComponent);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetRelativeScale3D(FVector(0.4));
		//
	}
}


// Called when the game starts or when spawned
void ANasunhwan::BeginPlay()
{
	Super::BeginPlay();
	UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/LMH/Effect/smoke_MH.smoke_MH"));
	smoke->SetAsset(NiagaraSystem);

	/*//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("spawn actor")), true, FVector2D(1, 1));*/
}

// Called every frame
void ANasunhwan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsStop)
	{
		float dist = FVector::Distance(GetActorLocation(),TargetPos);
		if (dist<10)
		{
			IsStop=true;

			/*ANasunhwa* nsh= UGameplayStatics::GetActorOfClass(GetWorld(), ANasunhwan::StaticClass());*/
			ToButtomMH();

		}
		else {

			SetActorLocation((Speed*GetActorForwardVector()*DeltaTime)+GetActorLocation());
		}
	}
}

