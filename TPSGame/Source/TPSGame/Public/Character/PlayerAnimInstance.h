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

	//montage
	UPROPERTY(EditAnywhere, blueprintreadWrite,category= Montage,meta =(Allowprivateaccess=true))
	UAnimMontage* WalkMontage;
	UPROPERTY(EditAnywhere, blueprintreadWrite, category = Montage, meta = (Allowprivateaccess = true))
	UAnimMontage* IdleMontage;

	
	
public:
	UPROPERTY(BlueprintReadOnly)
	FRotator HeadRotater;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWalk;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayer=false;
public:
	void Init(APlayerCharacter* value){Player=value;}
	void SetPlayer(){bIsPlayer=true;}
//playMotange
	void PlayIdleMontage();
	void PlayWalkMontage();
	
//fsm
	UFUNCTION(BlueprintCallable)
	void SetWalkState();
	UFUNCTION(BlueprintCallable)
	void SetIdleState();
	
	UFUNCTION(BlueprintCallable)
	void SetHeadRotator(FRotator value){HeadRotater=value;}
	UFUNCTION(BlueprintCallable)
	FRotator GetHeadRotator(){return HeadRotater;}

	UFUNCTION()
	bool GetbIsWalk(){return bIsWalk;}
};
