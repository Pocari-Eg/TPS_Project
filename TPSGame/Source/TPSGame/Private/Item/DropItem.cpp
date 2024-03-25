// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DropItem.h"

#include "TPSGameInstance.h"
#include "Character/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("Coliision"));
	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent=SphereComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetRelativeLocation(FVector::ZeroVector);
	SphereComponent->SetCollisionProfileName("DropItem");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADropItem::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADropItem::OnEndOverlap);
	InitItem(101);
}

void ADropItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<APlayerCharacter>(OtherActor))
	{
		APlayerCharacter* Player=Cast<APlayerCharacter>(OtherActor);
		if(Player->bIsPlayer)Player->OnClosedItem(this);
	}
}

void ADropItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(Cast<APlayerCharacter>(OtherActor))
	{
		APlayerCharacter* Player=Cast<APlayerCharacter>(OtherActor);
		if(Player->bIsPlayer)Player->OnFarItem();
	}
}

void ADropItem::InitItem(int newid)
{
	ItemId=newid;
	Data=Cast<UTPSGameInstance>(GetGameInstance())->GetItemData(ItemId);
	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
}

// Called every frame
void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
