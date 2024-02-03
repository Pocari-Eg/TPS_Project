// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/**
 * 
 */
class UTextBlock;
class APlayerCharacter;
UCLASS()
class TPSGAME_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

#pragma region var
private:
	UPROPERTY()
	APlayerCharacter* Player;

	//widget
	UPROPERTY()
	UTextBlock* HPText;
public:

#pragma  endregion var


#pragma region func
private:
	UFUNCTION()
void UpdateHPWidget();

public:

void BindPlayer(APlayerCharacter* value);

protected:
	// 위젯을 초기화
	virtual void NativeConstruct() override;
	//위젯 업데이트	
#pragma  endregion func

	

	
};
