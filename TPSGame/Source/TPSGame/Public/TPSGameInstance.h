// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "Engine/GameInstance.h"

#include "TPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:UPROPERTY()
	FString m_NickName;
	
public:
	UFUNCTION()
	void SetNickName(FString value){m_NickName=value;}
	//UFUNCTION()
	//FString GetNickNameW(){return m_NickName;}
	UFUNCTION()
		FString GetNickName() { return m_NickName; }
};
