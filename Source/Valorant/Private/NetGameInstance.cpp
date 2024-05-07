// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UNetGameInstance::UNetGameInstance()
{
	mySessionName=TEXT("Implant");

}

void UNetGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* subsys = IOnlineSubsystem::Get())
	{
		// Online Session Interface API 접근용 인스턴스 가져오기
		sessionInterface = subsys->GetSessionInterface();

		//// 세션 이벤트에 함수 바인딩하기
		//파라미터 2개쓰는 일반 델리게이트

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreatedMySession);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindOtherSessions);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSelectedSession);

	}
}
void UNetGameInstance::CreateMySession(FText roomName, int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;

	// 1. LAN 연결인지 DEDICATED 연결인지 설정한다.
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. 검색 가능한 방으로 설정한다.
	sessionSettings.bShouldAdvertise = true;

	// 3. 자기 정보를 전달될 수 있게 설정한다.
	sessionSettings.bUsesPresence = true;

	// 4. 다른 유저의 중간 입장을 허용한다.
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// 5. 입장 가능 인원을 설정한다.
	sessionSettings.NumPublicConnections = playerCount;

	//// 6. 세션에 추가 설정을 넣는다.
	sessionSettings.Set(FName("ROOM_NAME"), roomName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	bool isSuccess = sessionInterface->CreateSession(0, FName(roomName.ToString()), sessionSettings);
	UE_LOG(LogTemp, Warning, TEXT("Session Create Result: %s"), isSuccess ? *FString("Success") : *FString("Failed..."));


}
void UNetGameInstance::OnCreatedMySession(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//listen -> listen server의 주체로서 서버트레블
		bool result = GetWorld()->ServerTravel("/Game/AlphaServer/valorant_beta?Listen", true);
		UE_LOG(LogTemp, Warning, TEXT("Travel Result: %s"), result ? *FString("Success") : *FString("Failed..."));
	}
}

void UNetGameInstance::FindOtherSession()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. 세션 검색을 LAN으로 할 것인지 여부를 설정한다.
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. 세션 쿼리(query) 설정한다.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 3. 세션의 검색량을 설정한다.
	sessionSearch->MaxSearchResults = 50;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindOtherSessions(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// 검색된 세션 목록을 가져온다.
		if(sessionSearch == nullptr) return;

		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("Find Sessions: %d"), searchResults.Num());

		for (int32 i = 0; i < searchResults.Num(); i++)
		{
			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			FString hostName;
			searchResults[i].Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
			int32 openNumber = searchResults[i].Session.NumOpenPublicConnections;
			int32 maxNumber = searchResults[i].Session.SessionSettings.NumPublicConnections;
			int32 pingSpeed = searchResults[i].PingInMs;

			UE_LOG(LogTemp, Warning, TEXT("Room Name: %s, HostName: %s, OpenNumber: %d, MaxNumber: %d, Ping Speed: %d"), *roomName, *hostName, openNumber, maxNumber, pingSpeed);

			// 구조체 변수에 찾은 세션 정보를 입력한다.
			FSessionSlotInfo slotInfo;
			slotInfo.Set(roomName, hostName, FString::Printf(TEXT("(%d/%d)"), maxNumber - openNumber, maxNumber), pingSpeed, i);

			// 세션 정보를 델리게이트로 전파한다.
			onSearchCompleted.Broadcast(slotInfo);
		}

		onFindButtonActivation.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search failed..."));
	}
}

void UNetGameInstance::JoinSelectedSession(int32 index)
{
	sessionInterface->JoinSession(0, FName(mySessionName), sessionSearch->SearchResults[index]);
}

void UNetGameInstance::OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
	{
		UE_LOG(LogTemp, Warning, TEXT("Success: %s"), *sessionName.ToString());

		APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

		if (playerCon != nullptr)
		{
			// join된 세션 호스트의 ServerTravel 된 맵 주소를 받아온다.
			FString url;
			sessionInterface->GetResolvedConnectString(sessionName, url);
			UE_LOG(LogTemp, Warning, TEXT("Connection URL: %s"), *url);

			// 주소를 받았다면, 그 주소에 따라서 맵 이동한다.
			if (!url.IsEmpty())
			{
				playerCon->ClientTravel(url, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("SessionIsFull"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("SessionDoesNotExist"));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("CouldNotRetrieveAddress"));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("AlreadyInSession"));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("UnknownError"));
		break;
	default:
		break;
	}
}
