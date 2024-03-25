// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"



class ABullet;
UCLASS( ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UWeaponComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

#pragma region Component
public:
UPROPERTY(EditAnywhere, blueprintreadwrite, category = data)
class UStaticMeshComponent* MeshComponent;
#pragma endregion Component

#pragma  region Bullet

	//bullet class   f=5, s= 6  ffsbullet =5.56mm 
	UPROPERTY()
	TSubclassOf<ABullet> ffsBullet;
#pragma  endregion Bullet
	
#pragma region Data
public:
UPROPERTY()
 class APlayerCharacter* Player;

public:
	UPROPERTY(EditAnywhere)
		int32 Range = 1000;
private:
	bool bIsEquip=false;
#pragma endregion Data
		
	//function
public:

UFUNCTION(BlueprintCallable)
void SetVisible(bool Set);
		
#pragma region GetSet
	void bindPlayer(APlayerCharacter* Value);
		
#pragma endregion GetSet

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetIdleGrip();
	UFUNCTION()
	void SetWalkGrip();


    void EquipWeapon(int32 id);
	
	void SetbIsEquip(bool value){bIsEquip=value;}
	bool GetbIsEquip(){return bIsEquip;}

	bool Fire(class UCameraComponent* Camera,class USpringArmComponent* SpringArm);
};
