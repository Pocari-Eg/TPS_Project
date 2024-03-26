// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/weapon/Bullet.h"

#include "Projects.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CapsuleComponent=CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    MeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	RootComponent=CapsuleComponent;
	CapsuleComponent->SetCollisionProfileName("Bullet");
	MeshComponent->SetupAttachment(CapsuleComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PHITPARTICLE(TEXT("ParticleSystem'/Game/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact'"));
	if(PHITPARTICLE.Succeeded())PlayerHitParticle=PHITPARTICLE.Object;


	static ConstructorHelpers::FObjectFinder<UParticleSystem> OHITPARTICLE(TEXT("ParticleSystem'/Game/WeaponEffects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if(OHITPARTICLE.Succeeded())ObjectHitParticle=OHITPARTICLE.Object;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComponent->OnComponentHit.AddDynamic(this,&ABullet::HitPlayer);
}

void ABullet::HitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<APlayerCharacter>(Hit.GetActor()))
	{
		auto HitPlayer = Cast<APlayerCharacter>(Hit.GetActor());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),PlayerHitParticle,Hit.Location,FRotator::ZeroRotator,FVector(2.0f,2.0f,2.0f));
		HitPlayer->Hit(Damage);
	
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ObjectHitParticle,Hit.Location);
	}


	
	Destroy();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timer+=DeltaTime;
	if(timer>=lifeTime)
	{
		Destroy();
	}

}

void ABullet::Shoot()
{

}

