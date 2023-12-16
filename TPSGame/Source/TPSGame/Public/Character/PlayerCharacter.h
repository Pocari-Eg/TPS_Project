// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TPSGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
protected:

	class UPlayerAnimInstance* PlayerAnim;
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Handles input for moving forward and backward.
	UFUNCTION()
		void MoveForward(float value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float value);

	UFUNCTION()
	void Trun(float value);
	UFUNCTION()
	void LookUp(float value);


public:
	//debug
	UPROPERTY(EditAnywhere)
	float CameraSpeed;
	
};
