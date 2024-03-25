// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemManager.h"

#include "TPSGameInstance.h"
#include "Item/DropItem.h"
#include "Kismet/GameplayStatics.h"

AItemManager::AItemManager()
{
	PrimaryActorTick.bCanEverTick = false;

	
}

void AItemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	auto instance=Cast<UTPSGameInstance>(GetGameInstance());
	instance->SetItemManager(this);
	TArray<AActor*> temp;
   UGameplayStatics::GetAllActorsOfClass(GetWorld(),DropItemClass,temp);
	
	for(auto it=temp.begin();it!=temp.end();++it)
	{

		ADropItem* item=Cast<ADropItem>(*it);
		DropItemList.Add(item);

	}
	
}
void AItemManager::InitItem(TArray<int32>& InitList)
{
	for (int32 Index = 0; Index != InitList.Num(); ++Index)
	{
		DropItemList[Index]->InitItem(InitList[Index]);
	}
}

void AItemManager::DeleteItem(int32 idx)
{

	auto deleteItem=DropItemList[idx];
	DropItemList.RemoveAt(idx);
	deleteItem->Destroy();
}

void AItemManager::AddItem(ADropItem* item)
{
	DropItemList.Add(item);
}

const int32 AItemManager::GetIndex(ADropItem* item)
{
	return DropItemList.Find(item);
}

ADropItem* AItemManager::GetItem(int32 idx)
{
	return DropItemList[idx];
}
