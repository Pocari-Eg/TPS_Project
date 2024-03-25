// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "Character/PlayerCharacter.h"
#include "Engine/GameInstance.h"
#include "Network/NetworkBase.h"
#include "Network/ClientThread.h"


#include "TPSGameInstance.generated.h"

/**
 * 
 */

struct FItemData;
struct FWeaponData;
class UDataTable;
class APlayerCharacter;
class AItemManager;

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
	TMap<FString,int32> PlayerIndex;
	UPROPERTY()
	TSubclassOf<APlayerCharacter> PlayerClass;
	UPROPERTY()
	APlayerCharacter* Player;

	UPROPERTY()
	AItemManager* ItemManager;

//DataTable
	UDataTable* ItemData;
	UDataTable* WeaponData;

	

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

	void DeletePlayer(FString name);
	
	void SortPlayerList();

	void UpdateUserPos(const std::vector<FReplication>& data);
	
	ClientThread* GetClient(){return Player->GetClientThread();}
	int32 GetPlayerIndex(FString name);
	APlayerCharacter* GetPlayer(int32 idx);
	
	UFUNCTION()
	const int32 GetUserCount();

	
	void AddPlayerInGame(const TArray<FString>& addPlayer);
	
   UFUNCTION(BlueprintCallable)
	void OutGame();

	//Datatable
	FItemData* GetItemData(int32 ItemId);
	FWeaponData* GetWeaponData(int32 Code);

	void SetItemManager(AItemManager* temp){ItemManager=temp;}
	AItemManager* GetItemManager(){return ItemManager;}
	
private:
	bool AlreadyInList(FString name);
};
