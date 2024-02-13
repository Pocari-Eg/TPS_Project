// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */

class APlayerCharacter;
UCLASS()
class TPSGAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	APlayerCharacter* Player;

	
	
public:
	UPROPERTY(BlueprintReadOnly)
	FRotator verticalRotater;
	UPROPERTY(BlueprintReadOnly)
	FRotator horizontalRotater;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsWalk;
	UPROPERTY(BlueprintReadWrite)
	bool bIsRun;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayer=false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsEquip=false;
public:
	void Init(APlayerCharacter* value){Player=value;}
	void SetPlayer(){bIsPlayer=true;}

	
//fsm
	UFUNCTION(BlueprintCallable)
	void SetWalkState();
	UFUNCTION(BlueprintCallable)
	void SetIdleState();
	
	UFUNCTION(BlueprintCallable)
	void SetverticalRotator(FRotator value){verticalRotater=value;}
	UFUNCTION(BlueprintCallable)
	FRotator GetverticalRotator(){return verticalRotater;}

	UFUNCTION(BlueprintCallable)
	void SethorizontalRotator(FRotator value){horizontalRotater=value;}
	UFUNCTION(BlueprintCallable)
	FRotator GethorizontalRotator(){return horizontalRotater;}
	
	UFUNCTION()
	bool GetbIsWalk(){return bIsWalk;}

	UFUNCTION()
	void SetbIsRun(bool value){bIsRun=value;}
	UFUNCTION(BlueprintCallable)
	bool GetbIsRun(){return bIsRun;}

	UFUNCTION()
	void SetbIsEquip(bool value){bIsEquip=value;}
	UFUNCTION(BlueprintCallable)
	bool GetbIsEquip(){return bIsEquip;}
};
