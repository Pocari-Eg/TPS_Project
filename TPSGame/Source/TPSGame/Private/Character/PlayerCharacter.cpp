// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "BlendSpaceAnalysis.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Character/PlayerAnimInstance.h"
#include "Math.h"
#include "TPSGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Blueprint/UserWidget.h"

#include "Widget/PlayerHud.h"
#include "Engine/DamageEvents.h"
#include "Character/weapon/WeaponComponent.h"
#include "Item/DropItem.h"
#include "Item/ItemManager.h"
#include "Network/ClientThread.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(22.0f, 72.0f);


	const ConstructorHelpers::FObjectFinder<USkeletalMesh>CharacterMesh(TEXT("SkeletalMesh'/Game/Characters/Female/SK_Female.SK_Female'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -73.0f));
		GetMesh()->SetRelativeScale3D(FVector(0.8f,0.8f, 0.8f));
		
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		ConstructorHelpers::FClassFinder<UAnimInstance>CharacterAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/BP_PlayerAnim.BP_PlayerAnim_c'"));
		if (CharacterAnimInstance.Succeeded())
		{
			GetMesh()->SetAnimClass(CharacterAnimInstance.Class);
		}


	}

    GetCapsuleComponent()->SetCollisionProfileName("Character");
	

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 110.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
   

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetRelativeLocation(FVector(0.0f,27.0f,70.0f));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetFieldOfView(105.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate=FRotator(0.f,360.f,0.f);


	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("PlayerWeapon"));
	FName WeaponSocket(TEXT("WeaponRight"));
	Weapon->MeshComponent->SetupAttachment(GetMesh(), WeaponSocket);
	//curve
    	const ConstructorHelpers::FObjectFinder<UCurveFloat>RotationCurveData(TEXT("/Script/Engine.CurveFloat'/Game/data/RotatingCurve.RotatingCurve'"));
    	if (RotationCurveData.Succeeded())
    	{
    		RotationCurve = RotationCurveData.Object;
     
    	}
    
    	RotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeLine"));


	//network
	RepliData={0.0f,0.0f,72.0f,0.0f,IDLE};

	//widget
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBP(TEXT("/WidgetBlueprint'/Game/Widget/HUD/BP_PlayerHUD.BP_PlayerHUD_C'"));
	if (WidgetBP.Succeeded()) {

		PlayerWidgetBP = WidgetBP.Class;
	}

	LimitAngle = 55.0f;
	CameraSpeed = 10.0f;


	//sound
	FireSoundEvent = UFMODBlueprintStatics::FindEventByName("event:/SFX/Weapon/SFX_M4A1Fire");
	HitSoundEvent = UFMODBlueprintStatics::FindEventByName("event:/SFX/Voice/SFX_Fm_Hit");
}

void APlayerCharacter::UpdateUIOnMainThread(const FString& Message)
{
	// 메인 스레드에서 TakeDamage 호출
	float DamageAmount = FCString::Atof(*Message);
	FDamageEvent DamageEvent;
	TakeDamage(DamageAmount, DamageEvent, nullptr, nullptr);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitFsmInstance();
	PlayerAnim=Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	PlayerAnim->Init(this);
	PlayerAnim->SetIdleState();

	PlayerAnim->OnShoot.AddUObject(this, &APlayerCharacter::Fire);
	instance=Cast<UTPSGameInstance>(GetGameInstance());

	FireSound = new SoundManager(FireSoundEvent, GetWorld());
	FireSound->SetVolume(0.5f);

	HitSound = new SoundManager(HitSoundEvent, GetWorld());
	HitSound->SetVolume(1.0f);
	
	Weapon->bindPlayer(this);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (bIsPlayer)
	{
		client->Stop();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsRDelay)
	{
		RLimitTimer+=DeltaTime;
		if(RLimitTimer>=RLimitTime)
		{
			bIsFollow=true;
			bIsRDelay=false;
		}
	}


	if (!bIsPlayer) {
		PositionSync(DeltaTime);
	}	
	if(bIsAutoShoot)
	{
		ShootTimer+=DeltaTime;
		if(ShootTimer>=ShootLimit)
		{
			ShootTimer=0.0f;
			Weapon->Fire(FollowCamera,CameraBoom);
		}
	}
	if(bIsFire&&bIsPlayer)
	{
		PlayFireAnim();
	}

		if(!bIsCameraControl)
    	{
    		UpperFollowCamera();
    		if (PlayerAnim->GetbIsWalk())CameraFollowPlayer();
    	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Trun);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("Run", IE_Pressed, this	, &APlayerCharacter::Run, true);
	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("Run", IE_Released, this, &APlayerCharacter::Run, false);
	
	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("Fire", IE_Pressed, this	, &APlayerCharacter::SetFire, true);
	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("Fire", IE_Released, this, &APlayerCharacter::SetFire, false);
	
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &APlayerCharacter::Action);

	PlayerInputComponent->BindAction("Camera", IE_Pressed, this, &APlayerCharacter::OnCameraControl);
	PlayerInputComponent->BindAction("Camera", IE_Released, this, &APlayerCharacter::OffCameraControl);

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float Damage=DamageAmount;

	// 피해 계산 및 적용

	HitSound->SoundPlay2D();
	// 사망 여부 확인
	 if(HP - Damage<=0)
	 {
		 HP=0.0f;
	 	client->Stop();
        PlayerHud->OpenDeathWidget();
	 }
	else
	{
		HP -= Damage;
	}
	 if(bIsPlayer)OnHpChanged.Broadcast();



	return DamageAmount;
}


void APlayerCharacter::MoveForward(float value)
{
	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, value);

	PlayerAnim->MoveDirection(value);
}

void APlayerCharacter::MoveRight(float value)
{
	FVector FowardVector = GetActorForwardVector();
	FVector Direction=FowardVector.RotateAngleAxis(90.0f,FVector::UpVector);
	AddMovementInput(Direction, value);

	PlayerAnim->LowerRotation=value*30;
}

void APlayerCharacter::Trun(float value)
{
	//deltatime과 카메라 회전 값을 통해 마우스가 움직이면 좌우 회전
	float time=GetWorld()->DeltaTimeSeconds;
	float m_value=value*CameraSpeed*time;
	AddControllerYawInput(m_value);
}

void APlayerCharacter::LookUp(float value)
{
	float time=GetWorld()->DeltaTimeSeconds;
	float m_value=value*CameraSpeed*time;
	AddControllerPitchInput(m_value);

}

void APlayerCharacter::UpperFollowCamera()
{

	FVector CameraFowardVecetor=FollowCamera->GetForwardVector();
	
	float r,p,y;

	float CurYaw=GetActorRotation().Yaw;
	float sin=FMath::Sin(FMath::DegreesToRadians(CurYaw));
    float cos=FMath::Cos(FMath::DegreesToRadians(CurYaw));


   FRotator CameraRotator=CameraFowardVecetor.Rotation();
	p=CameraRotator.Pitch*cos;
	r=CameraRotator.Pitch*sin;
	y=CameraRotator.Yaw-CurYaw;
	

   PlayerAnim->SetFollowRotator(FRotator(p,y,r*-1.0f));
	float degree=FMath::Abs(y);
	//한변에 하체 및 플레이어의 방향을 회전
	if(degree>=LimitAngle&&bIsFollow)
	{
		CalcSign(GetActorForwardVector(),CameraFowardVecetor);
		PrevRotation=0.0f;
		RotationTimeLine->PlayFromStart();
	}
}

void APlayerCharacter::CameraFollowPlayer() 
{
	FVector CameraV=FollowCamera->GetForwardVector();
	FVector PlayerV =GetActorForwardVector();
	


	PlayerV.Z=0.0f;
	CameraV.Z=0.0f;
		
	PlayerV.Normalize();
	CameraV.Normalize();
		
	FQuat RotationQuat = Math::VectorA2BRotation(PlayerV, CameraV);
	
	AddActorWorldRotation(RotationQuat);
}

void APlayerCharacter::CalcSign(FVector PlayerTemp, FVector CameraTemp)
{	
	
	CameraTemp.Z=0.0f;
	PlayerTemp.Z=0.0f;

	CameraTemp.Normalize();
	PlayerTemp.Normalize();
	
	float dot=  FVector::DotProduct(CameraTemp,PlayerTemp);
	float degree= UKismetMathLibrary::DegAcos(dot);
	//외적을 통해 플레이어 벡터를 중심으로 카메라가 왼쪽을 향하면 -, 오른쪽을 향하면 +를 부호로 설정해 각도 변환
	FVector OutProduct = FVector::CrossProduct(PlayerTemp, CameraTemp);
	Sign = UKismetMathLibrary::SignOfFloat(OutProduct.Z);
	
}

void APlayerCharacter::Rotating(float Value)
{
	float TimeLineValue = Value;
	float NewRotationValue = (LimitAngle* TimeLineValue) - PrevRotation;
	PrevRotation += NewRotationValue;
	NewRotationValue*=Sign;
	AddActorWorldRotation(FRotator(0.0f, NewRotationValue, 0.0f));
}

void APlayerCharacter::FinishRotation()
{

	PlayerAnim->SetFollowRotator(FRotator::ZeroRotator);
	PrevRotation = 0.0f;
}

void APlayerCharacter::PositionSync(float DeltaTime)
{
	switch (RepliData.state)
	{
	case State::IDLE:
		PlayerAnim->SetIdleState();
		break;
	case State::WALK:
		PlayerAnim->SetWalkState();
		break;
	case State::RUN:
	 	PlayerAnim->SetRunState();
	 	break;	
	}

	// 현재 위치와 목표 위치를 가져옴
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = FVector(RepliData.PosX,RepliData.PosY,RepliData.PosZ);
	// 선형 보간을 사용하여 부드러운 이동
	FVector LerpedLocation = FMath::Lerp(CurrentLocation, TargetLocation, Time*DeltaTime);

	FVector forwardVector=GetActorForwardVector();

	FVector TargetVector=LerpedLocation-CurrentLocation;

	float DotProduct = FVector::DotProduct(forwardVector.GetSafeNormal(), TargetVector.GetSafeNormal());

	PlayerAnim->MoveDirection(DotProduct);


	
	
	//PlayerAnim->MoveDirection(value);
	
	// 현재 회전과 목표 회전을 가져옴
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = FRotator::ZeroRotator; // 서버에서 받은 회전 등
	TargetRotation.Yaw=RepliData.RotZ;
	// 선형 보간을 사용하여 부드러운 회전
	FRotator LerpedRotation = FMath::Lerp(CurrentRotation, TargetRotation,Time*DeltaTime);



	
	// 부드러운 이동 적용
	SetActorLocation(LerpedLocation);
	// 부드러운 회전 적용
	SetActorRotation(LerpedRotation);
}

void APlayerCharacter::InitRotatingCurve()
{

	RotationCallBack.BindUFunction(this, FName("Rotating"));
	RotationFinishCallback.BindUFunction(this, FName("FinishRotation"));

	RotationTimeLine->SetTimelineFinishedFunc(RotationFinishCallback);
	RotationTimeLine->AddInterpFloat(RotationCurve, RotationCallBack);
	
	float Min = 0.0f;
	float Max = 0.0f;
	RotationCurve->GetTimeRange(Min, Max);
	RotationTimeLine->SetTimelineLength(Max);
}

void APlayerCharacter::InitFsmInstance()
{
	
	FSMInstance = NewObject<UPlayerFSM>();
	FSMInstance->SetPlayer(this);
	FSMInstance->ChangeState(UIdleState::GetInstance());
}

void APlayerCharacter::InitPlayerHud()
{
	if (PlayerWidgetBP != nullptr)
	{
		PlayerHud =Cast<UPlayerHud>(CreateWidget<UPlayerHud>(GetWorld(), PlayerWidgetBP));
		PlayerHud->AddToViewport();
		PlayerHud->BindPlayer(this);
	}
}

void APlayerCharacter::InitPlayer()
{
	InitPlayerHud();
	InitRotatingCurve();

}

void APlayerCharacter::SetPlayerCharacter(const FString&  name)
{
	NickName=name;
	if(bIsDebug)TLOG_E(TEXT("SetPlayerCharacter"));
	client = new ClientThread();
	client->BindPlayer(NickName,this);
	client->StartThreads();
	
	
	bIsPlayer = true;
	InitPlayer();
	PlayerAnim->SetPlayer();
}

void APlayerCharacter::SetReplidata(const FReplication value)
{
	RepliData=value;
}

void APlayerCharacter::Fire()
{
	if(!Weapon->Fire(FollowCamera,CameraBoom))
	{
		if(bIsDebug)TLOG_E(TEXT("Not Equip Weapon"));
	}
	else
	{
		FireSound->SoundPlay3D(GetActorTransform());
	}
	
}

void APlayerCharacter::PlayFireAnim()
{
	if(bIsPlayer)
	{
		PlayerAnim->PlayFireMonatage();
	}
}

void APlayerCharacter::Hit(int32 Damage)
{
	std::string index =TCHAR_TO_UTF8(*FString::FromInt(instance->GetPlayerIndex(NickName)));
	std::string damage=TCHAR_TO_UTF8(*FString::FromInt(Damage));
	std::string data=":hit "+index+","+damage;


	
   instance->GetClient()->Send(data);
}

void APlayerCharacter::Run(bool value)
{
	GetCharacterMovement()->MaxWalkSpeed = value ? 500.f : 330.f;
	PlayerAnim->SetbIsRun(value);
}

void APlayerCharacter::SetFire(bool value)
{
	bIsFire=value;
}

void APlayerCharacter::Action()
{
	switch (ReadyAction)
	{
	case EAction::PICKUP:
		PickUpItem();
		break;
	case EAction::OPEN:
		break;
	case EAction::RIDE:
		break;
	case EAction::NONE:
		break;
			
		
	}
}

void APlayerCharacter::OnCameraControl()
{
	bIsCameraControl=true;
	OriginCameraRotator=GetControlRotation();
	OriginCameraYaw=GetActorRotation().Yaw;
	CameraBoom->bInheritPitch=false;
	bIsFollow=false;
	
}
void APlayerCharacter::OffCameraControl()
{
	
	GetController()->SetControlRotation(OriginCameraRotator);
	PlayerAnim->SetFollowRotator(FRotator::ZeroRotator);
	CameraBoom->bInheritPitch=true;
	bIsCameraControl=false;
	
	RLimitTimer=0.0f;
	bIsRDelay=true;
}

void APlayerCharacter::OnClosedItem(ADropItem* item)
{
	ReadyAction=EAction::PICKUP;
	closedItem=item;
	if(closedItem!=nullptr)
	PlayerHud->OnActionWidget(closedItem->GetData()->ItemName);
	
}

void APlayerCharacter::OnFarItem()
{
	ReadyAction=EAction::NONE;
	PlayerHud->OffActionWidget();
	closedItem=nullptr;
}

void APlayerCharacter::EquipItem(int32 itemidx)
{
	AItemManager* ItemManager=instance->GetItemManager();
	ADropItem* item=ItemManager->GetItem(itemidx);

	switch (item->GetData()->ItemType)
	{
	
	case EItemType::WEAPON:
		PickUpWeapon(item->GetData()->ItemID);
		break;
	case EItemType::ARMOR:
		break;
	case EItemType::ACTVIE:
		break;
	}

	ItemManager->DeleteItem(itemidx);
	
}


void APlayerCharacter::PickUpItem()
{
	if(closedItem!=nullptr)
	{	
		std::string Itemidx =TCHAR_TO_UTF8(*FString::FromInt(instance->GetItemManager()->GetIndex(closedItem)));
		std::string Playeridx=TCHAR_TO_UTF8(*FString::FromInt(instance->GetPlayerIndex(NickName)));
		std::string data=":tpick "+Playeridx+","+Itemidx;
		client->Send(data);
		closedItem=nullptr;
	}
}

inline void APlayerCharacter::PickUpWeapon(int id)
{
   Weapon->EquipWeapon(id);
	PlayerAnim->SetbIsEquip(true);
}
