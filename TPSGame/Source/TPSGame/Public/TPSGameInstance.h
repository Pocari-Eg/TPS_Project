// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "Engine/GameInstance.h"
#include "Network/NetworkBase.h"
#include "TPSGameInstance.generated.h"

/**
 * 
 */

class APlayerCharacter;
UCLASS()
class TPSGAME_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	FString m_NickName="Pocari";
	UPROPERTY()
	TMap<FString,APlayerCharacter*> PlayerList;
	UPROPERTY()
	TSubclassOf<APlayerCharacter> PlayerClass;

public:

	//constructor
	UTPSGameInstance();
  


	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();
	
	//network
	UFUNCTION(BlueprintCallable)
	void SetNickName(FString value){m_NickName=value;}
	UFUNCTION()
	FString GetNickName() { return m_NickName; }
	
	UFUNCTION(BlueprintCallable)
	void AddPlayUser(const FString& name);
	const TMap<FString,APlayerCharacter*>& GetPlayerList();

	bool AlreadyInList(FString name);
	void SortPlayerList();

	void UpdateUserPos(const std::vector<FReplication>& data);
	
	
	UFUNCTION()
	const int32 GetUserCount();
};
