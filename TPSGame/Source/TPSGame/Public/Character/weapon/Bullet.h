// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()

class TPSGAME_API ABullet : public AActor
{
	GENERATED_BODY()
#pragma  region var	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ObjectHitParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* PlayerHitParticle;
	UPROPERTY()
	int32 Speed;


	UPROPERTY()
	float lifeTime=3.0f;
	UPROPERTY()
	float timer=0.0f;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= "DATA")
	int32 Damage;
#pragma  endregion var
	
#pragma  region func
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
UFUNCTION()
	void HitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Shoot();

#pragma  endregion func
	
};
