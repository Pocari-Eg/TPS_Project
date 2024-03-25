// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnShootDelegate);
class APlayerCharacter;
UCLASS()
class TPSGAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	APlayerCharacter* Player;

private:
	//montage
	UPROPERTY(EditAnywhere, blueprintreadWrite, category = Montage, meta = (Allowprivateaccess = true))
	UAnimMontage* FireMontage;
	
	
public:

	UPROPERTY(BlueprintReadWrite)
	FRotator FollowRotator;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsWalk;
	UPROPERTY(BlueprintReadWrite)
	bool bIsRun;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayer=false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsEquip=false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsForward;
	UPROPERTY(BlueprintReadWrite)
	float LowerRotation;
public:
	//delegate
	FOnShootDelegate OnShoot;


	
public:
	void Init(APlayerCharacter* value){Player=value;}
	void SetPlayer(){bIsPlayer=true;}

	//playMontage
	UFUNCTION()
	void PlayFireMonatage();
	
//fsm
	UFUNCTION(BlueprintCallable)
	void SetWalkState();
	UFUNCTION(BlueprintCallable)
	void SetIdleState();
	UFUNCTION(BlueprintCallable)
	void SetRunState();

	
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

	UFUNCTION()
	void MoveDirection(float value);

	UFUNCTION()
	void SetFollowRotator(FRotator data){FollowRotator=data;}
	UFUNCTION()
	FRotator GetFollowRotator(){return FollowRotator;}

	//notify
private:
	UFUNCTION()
	void AnimNotify_Shoot() const;
};
