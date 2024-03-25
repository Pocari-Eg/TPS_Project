// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Actor.h"
#include "DataTable/ItemTable.h"
#include "DropItem.generated.h"

UCLASS()
class TPSGAME_API ADropItem : public AActor
{
	GENERATED_BODY()
#pragma region var
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category="Data")
	class USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category="Data")
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category="Data")
	int32 ItemId;

	FItemData* Data;

#pragma  endregion var
	
	
#pragma region func

public:	
	// Sets default values for this actor's properties
	ADropItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//충돌 체크
	UFUNCTION()
virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
virtual	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void InitItem(int32 newid);
	FItemData* GetData(){return Data;}

#pragma endregion func
};
