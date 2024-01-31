// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "Character/FSM/PlayerFSM.h"


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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UWeaponComponent* Weapon;

	//fsm
	UPROPERTY()
	UPlayerFSM* FSMInstance;


	//widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PlayerWidgetBP;
public:
	UPROPERTY(BlueprintReadWrite)
	class UPlayerHud* PlayerHud;

	
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
	UPROPERTY()
	FString NickName;

	TQueue<FString> NameList;
	
	FReplication RepliData;

	UPROPERTY()
		bool bIsPlayer = false;
protected:
	class UPlayerAnimInstance* PlayerAnim;
	class UTPSGameInstance* instance;

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


	//synchronization
   UFUNCTION()
	void PositionSync(float DeltaTime);
	
UFUNCTION()
	void InitRotatingCurve();

	UFUNCTION()
	void InitFsmInstance();
	
public:
	UFUNCTION()
	void SetPlayerCharacter(const FString& name);

	UFUNCTION()
	UTPSGameInstance* GetInstance(){return instance;}
	
	FReplication GetRepliData(){return  RepliData;}
	void SetReplidata(const FReplication value){RepliData=value;}

	//fsm
	UPlayerFSM* GetFSMInstance(){return FSMInstance;}

	//weapon
	class UWeaponComponent* GetWeapon(){return Weapon;}

UFUNCTION(BlueprintCallable)
	void FIRE();
#pragma endregion Function
	
	//debug
	UPROPERTY(EditAnywhere,Category="DEBUG")
	float CameraSpeed;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	bool bIsDebug=true;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	float LimitAngle=90.0f;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	float Time=10.0f;
};
