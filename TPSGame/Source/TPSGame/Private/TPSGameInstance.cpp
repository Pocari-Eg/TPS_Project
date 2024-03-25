// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "ShaderPrintParameters.h"
#include  "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DataTable/ItemTable.h"
#include "Engine/DataTable.h"

FItemData* UTPSGameInstance::GetItemData(int32 ItemId)
{
	return ItemData->FindRow<FItemData>(*FString::FromInt(ItemId), TEXT(""));
}

FWeaponData* UTPSGameInstance::GetWeaponData(int32 Code)
{
	return WeaponData->FindRow<FWeaponData>(*FString::FromInt(Code), TEXT(""));
}

UTPSGameInstance::UTPSGameInstance()
{
	
	ConstructorHelpers::FClassFinder<APlayerCharacter>CharacterClass(TEXT("/Game/Characters/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if(IsValid(CharacterClass.Class))
	{
		PlayerClass=CharacterClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemData(TEXT("DataTable'/Game/data/Table/ItemTable.ItemTable'"));
	if (DT_ItemData.Succeeded())
	{
		ItemData = DT_ItemData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_WeaponData(TEXT("DataTable'/Game/data/Table/WeaponTable.WeaponTable'"));
	if (DT_WeaponData.Succeeded())
	{
		WeaponData = DT_WeaponData.Object;
	}
}

void UTPSGameInstance::SpawnPlayer()
{
    Player =	GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,75.0f),FRotator::ZeroRotator);
	Player->SetPlayerCharacter(GetNickName());
	PlayerList.Add(GetNickName(),Player);
	Player->SetNameEvent();
	SortPlayerList();
	GetWorld()->GetFirstPlayerController()->Possess(Player);

}

void UTPSGameInstance::AddPlayUser(const FString& name)
{
	
	if(!AlreadyInList(name))
	{
		FActorSpawnParameters Parameters;
		Parameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		 APlayerCharacter* NewPlayer=GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,77.0f),FRotator::ZeroRotator,Parameters);
		NewPlayer->NickName=name;
		NewPlayer->SetNameEvent();
		 PlayerList.Add(name,NewPlayer);
		 TLOG_W(TEXT("%s Player In Game "),*name);
	}
	
}

const TMap<FString,APlayerCharacter*>& UTPSGameInstance::GetPlayerList()
{
	return PlayerList;
}

void UTPSGameInstance::DeletePlayer(FString name)
{
	 APlayerCharacter* DeathPlayer = PlayerList.FindRef(name);
	
	if (DeathPlayer)
	{
		// 맵에서 플레이어 제거
		PlayerList.Remove(name);
		// 플레이어 제거
		DeathPlayer->Destroy();
	}
	SortPlayerList();
}

void UTPSGameInstance::AddPlayerInGame(const TArray<FString>& addPlayer)
{
	
	if (!addPlayer.IsEmpty())
	{
		
		for(auto it=addPlayer.begin();it!=addPlayer.end();++it)
		{
			AddPlayUser(*it);
		}
		SortPlayerList();
	}
}

void UTPSGameInstance::OutGame()
{
	PlayerList.Empty();
	PlayerIndex.Empty();
	UGameplayStatics::OpenLevel(this,"TestLogin");
}

bool UTPSGameInstance::AlreadyInList(FString name)
{
	if(PlayerList.IsEmpty())
		return false;

    if(PlayerList.Find(name))
    	return true;
	
	return false;
}

void UTPSGameInstance::SortPlayerList()
{
	PlayerList.KeySort([](FString A, FString B)
	{
		return A<B;
	});
	PlayerIndex.Empty();

	int index=0;
	for(auto it =PlayerList.begin();it!=PlayerList.end();++it)
	{
		PlayerIndex.Add(*it->Key,index);
		index++;
	}
}

void UTPSGameInstance::UpdateUserPos(const std::vector<FReplication>& data)
{
 auto it = PlayerList.begin();
	for(int i=0;i<data.size();i++)
	{
		if(it->Value!=nullptr&&it->Key!=m_NickName){
		it->Value->SetReplidata(data[i]);
			}
		 if(it!=PlayerList.end()) ++it;

	}
}


int32 UTPSGameInstance::GetPlayerIndex(FString name)
{
	int32 i =*PlayerIndex.Find(name);
	return i;
}

APlayerCharacter* UTPSGameInstance::GetPlayer(int32 idx)
{
     FString name=*PlayerIndex.FindKey(idx);

	return *PlayerList.Find(name);
	
	
}

const int32 UTPSGameInstance::GetUserCount()
{
	return PlayerList.Num();
}
