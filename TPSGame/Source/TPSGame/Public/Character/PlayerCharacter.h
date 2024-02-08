// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "Character/FSM/PlayerFSM.h"


#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

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


	//PlayerData
	UPROPERTY(VisibleAnywhere, Category = Data, meta = (AllowPrivateAccess = "true"))
	float HP=100;
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
	FReplication RepliData;

	UPROPERTY()
		bool bIsPlayer = false;

	//delegate
	FOnHpChangedDelegate OnHpChanged;
protected:
	class UPlayerAnimInstance* PlayerAnim;
	class UTPSGameInstance* instance;

#pragma region Function	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	
	void UpdateUIOnMainThread(const FString& Message);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
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
	UFUNCTION()
	void InitPlayerHud();

	void InitPlayer();

	
public:
	UFUNCTION()
	void SetPlayerCharacter(const FString& name);

	UFUNCTION()
	UTPSGameInstance* GetInstance(){return instance;}
	
	FReplication GetRepliData(){return  RepliData;}
	void SetReplidata(const FReplication value);
	 ClientThread* GetClientThread(){return client;}
	//fsm
	 UPlayerFSM* GetFSMInstance(){return FSMInstance;}
	

	//weapon
	 class UWeaponComponent* GetWeapon(){return Weapon;}

	void SetHP(int32 val){HP=val;}
	float GetHP(){return HP;}

UFUNCTION(BlueprintCallable)
	void FIRE();

	UFUNCTION(BlueprintCallable)
	void Hit(int32 Damage);

//network

	
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
	UPROPERTY(EditAnywhere,Category="DEBUG")
	bool bIsAutoShoot=false;
	UPROPERTY(EditAnywhere,Category="DEBUG")
	float ShootLimit=1.0f;
	float ShootTimer=0.0f;
};
