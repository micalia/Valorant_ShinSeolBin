// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "WinLoseWidget.h"
#include <Kismet/GameplayStatics.h>
#include "BaseCharacter.h"


ABattleGameModeBase::ABattleGameModeBase()
{
	/*static ConstructorHelpers::FClassFinder<UWinLoseWidget> tempWinLoseWidgetFactory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PSH/UI/WB_winLose.WB_winLose_C'"));
	if (tempWinLoseWidgetFactory.Succeeded()) {
		WinLoseWidgetFactory = tempWinLoseWidgetFactory.Class;
	}*/
}

void ABattleGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void ABattleGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//currTime +=DelayTime;

	//if (DelayTime < currTime) {
	//	playerCountArr.Empty();
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), playerCountArr);
	//	playerCount = playerCountArr.Num();
	//	currTime = 0;
	//}
}

AActor* ABattleGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> playerStarts;
	playerStarts.SetNum(2);

	for (TActorIterator<APlayerStart> it(GetWorld()); it; ++it)
	{
		APlayerStart* ps = *it;
		if (ps->ActorHasTag(FName("First")))
		{
			/*//GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, TEXT("playerStarts1111 !!!"), true, FVector2D(1, 1));*/
			playerStarts[0] = ps;
		}
		else if(ps->ActorHasTag(FName("Second")))
		{
			/*//GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Purple, TEXT("playerStarts 2222!!!"), true, FVector2D(1, 1));*/
			playerStarts[1] = ps;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("PlayerStart Number : %d"), callNumber);

	APlayerStart* chooseStart = playerStarts[callNumber];
	callNumber = (callNumber + 1) % playerStarts.Num();

	return chooseStart;
}