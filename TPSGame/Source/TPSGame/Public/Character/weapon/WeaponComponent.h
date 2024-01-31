// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"


class AActor;

UCLASS( ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent) )
class TPSGAME_API UWeaponComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

#pragma region Component
public:
UPROPERTY(EditAnywhere, blueprintreadwrite, category = data)
class USkeletalMeshComponent* MeshComponent;
#pragma endregion Component
		
#pragma region Data
public:
UPROPERTY()
 class APlayerCharacter* Player;

public:
	UPROPERTY(EditAnywhere)
		int32 Range = 1000;
	UPROPERTY(EditAnywhere)
	int32 Damage = 200;
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


	void Fire(class UCameraComponent* Camera,class USpringArmComponent* SpringArm);
};
