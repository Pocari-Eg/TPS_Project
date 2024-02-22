// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/DropItem.h"
#include "DropWeapon.generated.h"

/**
 * 
 */
class USkeletalMesh;
UCLASS()
class TPSGAME_API ADropWeapon : public ADropItem
{
	GENERATED_BODY()

	
public:
	ADropWeapon();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	 UStaticMesh* GetMesh() const;
};
