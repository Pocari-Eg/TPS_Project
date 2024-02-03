// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "ShaderPrintParameters.h"
#include  "Character/PlayerCharacter.h"

UTPSGameInstance::UTPSGameInstance()
{
	
	ConstructorHelpers::FClassFinder<APlayerCharacter>CharacterClass(TEXT("/Game/Characters/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if(IsValid(CharacterClass.Class))
	{
		PlayerClass=CharacterClass.Class;
	}
}

void UTPSGameInstance::SpawnPlayer()
{
    Player =	GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,75.0f),FRotator::ZeroRotator);
	Player->SetPlayerCharacter(GetNickName());
	PlayerList.Add(GetNickName(),Player);
	GetWorld()->GetFirstPlayerController()->Possess(Player);

}

void UTPSGameInstance::AddPlayUser(const FString& name)
{
	
	if(name!=m_NickName||!AlreadyInList(name))
	{
		FActorSpawnParameters Parameters;
		Parameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		 APlayerCharacter* NewPlayer=GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,77.0f),FRotator::ZeroRotator,Parameters);
		NewPlayer->NickName=name;
		 PlayerList.Add(name,NewPlayer);
		 TLOG_W(TEXT("%s Player In Game "),*name);
	}
	
}

const TMap<FString,APlayerCharacter*>& UTPSGameInstance::GetPlayerList()
{
	return PlayerList;
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
	PlayerIndex.Reset();

	int index=0;
	for(auto it =PlayerList.begin();it!=PlayerList.end();++it)
	{
		PlayerIndex.Add(*it->Key,index);
		index++;
	}


	for(auto it=PlayerIndex.begin();it!=PlayerIndex.end();++it)
	{
		TLOG_W(TEXT("%s : %d"),*it->Key,it->Value);
	}
}

void UTPSGameInstance::UpdateUserPos(const std::vector<FReplication>& data)
{
 auto it = PlayerList.begin();
	for(int i=0;i<data.size();i++)
	{
		it->Value->SetReplidata(data[i]);
		 if(it!=PlayerList.end()) ++it;

	}
}


int32 UTPSGameInstance::GetPlayerIndex(FString name)
{
	//PlayerList.Find(name);
	return 0;
}

const int32 UTPSGameInstance::GetUserCount()
{
	return PlayerList.Num();
}
