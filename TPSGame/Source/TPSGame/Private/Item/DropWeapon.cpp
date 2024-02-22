// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DropWeapon.h"
ADropWeapon::ADropWeapon()
{
	
}



void ADropWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADropItem::OnBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	TLOG_W(TEXT("OnBeginOverlap Weapon"));
}

void ADropWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ADropItem::OnEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	TLOG_W(TEXT("OnEndOverlap Weapon"));
}

UStaticMesh* ADropWeapon::GetMesh() const
{
	return StaticMeshComponent->GetStaticMesh();
}

