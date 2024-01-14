// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"


#include "PlayerCharacter.generated.h"
class ClientThread;
UCLASS()
class TPSGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	//components
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	

	//TimeLine
	// Rotation
    UPROPERTY()
	float PrevRotation;
	float Sign;
	UPROPERTY()
	UCurveFloat* RotationCurve;
	UPROPERTY()
	UTimelineComponent* RotationTimeLine;
	UPROPERTY()
	FOnTimelineFloat RotationCallBack;
	UPROPERTY()
	FOnTimelineEvent RotationFinishCallback;


	
                  
	//network;
	ClientThread* client;
	FString NickName;
protected:
	class UPlayerAnimInstance* PlayerAnim;

#pragma region Function	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Handles input for moving forward and backward.

private:

	//player move
	UFUNCTION()
	void MoveForward(float value);
	UFUNCTION()
	void MoveRight(float value);


	//player rotation
	UFUNCTION()
	void Trun(float value);
	UFUNCTION()
	void LookUp(float value);
	UFUNCTION()
	void HeadFollowing();
    UFUNCTION()
	void CameraFollowPlayer();

	UFUNCTION()
	float CalcDegree(FVector PlayerTemp,FVector CameraTemp);
	UFUNCTION()
	void Rotating(float Value);
	UFUNCTION()
	void FinishRotation();
	
UFUNCTION()
	void InitRotatingCurve();
	
public:
	UFUNCTION()
	void SetPlayerCharacter(FString name);

	
	//debug
	UPROPERTY(EditAnywhere,Category="DEBUG")
	float CameraSpeed;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	bool bIsDebug=true;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	 float LimitAngle=90.0f;

	


#pragma endregion Function
};
