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
    APlayerCharacter*  player =	GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,75.0f),FRotator::ZeroRotator);
	player->SetPlayerCharacter(GetNickName());
	PlayerList.Add(GetNickName(),player);
	GetWorld()->GetFirstPlayerController()->Possess(player);

}

void UTPSGameInstance::AddPlayUser(const FString& name)
{
	
	if(name!=m_NickName||!AlreadyInList(name))
	{
		 APlayerCharacter* NewPlayer=GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass,FVector(0.0f,0.0f,75.0f),FRotator::ZeroRotator);
		 PlayerList.Add(name,NULL);
		 SortPlayerList();
		 TLOG_E(TEXT("%s Player In Game "),*name);
	}



}

TMap<FString,APlayerCharacter*> UTPSGameInstance::GetPlayerList()
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
		return A>B;
	});
}
