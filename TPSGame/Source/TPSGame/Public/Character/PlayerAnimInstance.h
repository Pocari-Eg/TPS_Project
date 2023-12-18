// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FRotator HeadRotater;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWalk;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHeadRotator(FRotator value){HeadRotater=value;}
	UFUNCTION(BlueprintCallable)
	FRotator GetHeadRotator(){return HeadRotater;}

	UFUNCTION()
	bool GetbIsWalk(){return bIsWalk;}
};
