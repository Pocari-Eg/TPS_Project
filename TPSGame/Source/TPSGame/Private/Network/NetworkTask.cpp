// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkTask.h"
#include "Character/PlayerCharacter.h"
#include "TPSGameInstance.h"

FUpdateUITask::FUpdateUITask(APlayerCharacter* InPlayer, const FString& InMessage)
{
	Player=InPlayer;
	Message=InMessage;
}

void FUpdateUITask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	// 메인 스레드에서 UI 업데이트 호출
	if (Player.IsValid())
	{
		Player->UpdateUIOnMainThread(Message);
	}
}

FAddPlayerTask::FAddPlayerTask(UTPSGameInstance* Ininstance, const TArray<FString>& InPlayerList)
{

	instacne=Ininstance;
	PlayerList=InPlayerList;
}

void FAddPlayerTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{

	// 메인 스레드에서 UI 업데이트 호출
	if (instacne.IsValid())
	{
		instacne->AddPlayerInGame(PlayerList);
	}
}

FDeletePlayerTask::FDeletePlayerTask(UTPSGameInstance* Ininstacne, const FString& InPlayerName)
{

	instacne=Ininstacne;
	PlayerName=InPlayerName;
}

void FDeletePlayerTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{

	// 메인 스레드에서 UI 업데이트 호출
	if (instacne.IsValid())
	{
		instacne->DeletePlayer(PlayerName);
	}
}

