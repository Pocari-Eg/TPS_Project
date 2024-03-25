// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/weapon/WeaponComponent.h"

#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Character/PlayerCharacter.h"
#include "Character/weapon/Bullet.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MeshComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	// ...
	
	ConstructorHelpers::FClassFinder<ABullet>bulletClass(TEXT("Blueprint'/Game/Object/Weapon/BP_Bullet.BP_Bullet_C'"));
	if(IsValid(bulletClass.Class))
	{
		ffsBullet=bulletClass.Class;
	}
	
}



void UWeaponComponent::SetVisible(bool Set)
{
	MeshComponent->SetVisibility(Set);
}

void UWeaponComponent::bindPlayer(APlayerCharacter* Value)
{
	Player = Value;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::SetIdleGrip()
{
	
}

void UWeaponComponent::SetWalkGrip()
{
	
}

void UWeaponComponent::EquipWeapon(int32 id)
{
	
	FItemData* ItemData=	Player->GetInstance()->GetItemData(id);
	MeshComponent->SetStaticMesh(ItemData->StaticMesh);

	
	SetIdleGrip();
	SetbIsEquip(true);
}

bool UWeaponComponent::Fire(class UCameraComponent* Camera,class USpringArmComponent* SpringArm)
{
	
	/*
	FHitResult HitResult;
	FVector Start=	Camera->GetComponentLocation();
	FVector end=Start+(Camera->GetForwardVector()*(SpringArm->TargetArmLength+Range));


	if(GetWorld()->LineTraceSingleByChannel(HitResult,Start,end,ECC_Visibility))
	{

		DrawDebugLine(
			GetWorld(),
			Start,
			end,
			FColor::Green,
			false, 1.0f, 0, 3
		);
		TLOG_W(TEXT("%s"),*HitResult.GetActor()->GetActorLabel());
	}
	else
	{

		DrawDebugLine(
			GetWorld(),
			Start,
			end,
			FColor::Red,
			false, 1.0f, 0, 3
		);
		TLOG_E(TEXT("EMPTY"));
	}
*/

	if(!bIsEquip)return false;
	
	
	FVector CameraPos=	Camera->GetComponentLocation();
	FVector ShootPos=CameraPos+(Camera->GetForwardVector()*(SpringArm->TargetArmLength+100.0f));
	FRotator ShootRot=Camera->GetComponentRotation();

	FVector direction=Camera->GetForwardVector();
	direction.Normalize();
	auto bullet = GetWorld()->SpawnActor<ABullet>(ffsBullet,ShootPos,ShootRot);
	bullet->Shoot();
	Player->ShootEffectEvent();
	return true;
}


