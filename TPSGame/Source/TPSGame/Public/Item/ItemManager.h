// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//필드에 드랍되어있는 아이템을 관리하는 액터


#include "DropItem.h"
#include "TPSGame.h"
#include "GameFramework/Actor.h"
#include "ItemManager.generated.h"

/**
 * 
 */
class ADropItem;


UCLASS()
class TPSGAME_API AItemManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ADropItem*> DropItemList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropItem> DropItemClass;
public:	
	// Sets default values for this actor's properties
	AItemManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION()
	void InitItem(TArray<int>& InitList);
	UFUNCTION()
	void DeleteItem(int32 idx);
	UFUNCTION()
	void AddItem(ADropItem* item);
   UFUNCTION()
   const	int32 GetIndex(ADropItem* item);
	UFUNCTION()
		ADropItem* GetItem(int32 idx);
};

