// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "Engine/DataTable.h"
#include "ItemTable.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataBase)
	int ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataBase)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataBase)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ItemDataBase)
	int32 ItemCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataBase)
	UStaticMesh* StaticMesh;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponDataBase)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponDataBase)
	int Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponDataBase)
	int Range;

};
