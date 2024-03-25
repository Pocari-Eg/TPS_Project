// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkTask.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UNetworkTask : public UObject
{
	GENERATED_BODY()
	
};

class APlayerCharacter;
class UTPSGameInstance;

class FUpdateUITask: public FNonAbandonableTask
{
public:
	FUpdateUITask(APlayerCharacter* InPlayer, const FString& InMessage);
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);
	static TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FUpdateUITask, STATGROUP_ThreadPoolAsyncTasks);
	}

	// 수정된 부분
	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}

	// 수정된 부분
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
private:
	TWeakObjectPtr<APlayerCharacter> Player;
	FString Message;
};

class FAddPlayerTask: public FNonAbandonableTask
{
public:
	FAddPlayerTask(UTPSGameInstance* Ininstacne, const TArray<FString>& InPlayerList);
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);
	static TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAddPlayerTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	// 수정된 부분
	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}

	// 수정된 부분
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
private:
	TWeakObjectPtr<UTPSGameInstance> instacne;
	TArray<FString> PlayerList;
};

class FDeletePlayerTask: public FNonAbandonableTask
{
public:
	FDeletePlayerTask(UTPSGameInstance* Ininstacne, const FString& InPlayerName);
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);
	static TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDeletePlayerTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	// 수정된 부분
	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}

	// 수정된 부분
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
private:
	TWeakObjectPtr<UTPSGameInstance> instacne;
	FString PlayerName;
};

class FItemTask: public FNonAbandonableTask
{
public:
	FItemTask(APlayerCharacter* Player, const int32& ItemIndex);
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);
	static TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FItemTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	// 수정된 부분
	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}

	// 수정된 부분
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
private:
	TWeakObjectPtr<APlayerCharacter> m_Player;
	int32 m_ItemIndex;
};