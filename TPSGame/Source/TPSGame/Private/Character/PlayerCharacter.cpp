// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "BlendSpaceAnalysis.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Character/PlayerAnimInstance.h"
#include "Math.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(22.0f, 72.0f);


	const ConstructorHelpers::FObjectFinder<USkeletalMesh>CharacterMesh(TEXT("SkeletalMesh'/Game/Characters/Mesh/Rifle_Idle.Rifle_Idle'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -73.0f));
		GetMesh()->SetRelativeScale3D(FVector(0.8f,0.8f, 0.8f));
		
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		ConstructorHelpers::FClassFinder<UAnimInstance>CharacterAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/BP_PlayerAnim.BP_PlayerAnim_c'"));
		if (CharacterAnimInstance.Succeeded())
			GetMesh()->SetAnimClass(CharacterAnimInstance.Class);
	}


	

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 120.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.0f, 20.0f, 49.0f));


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
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

	//curve
    	const ConstructorHelpers::FObjectFinder<UCurveFloat>RotationCurveData(TEXT("/Script/Engine.CurveFloat'/Game/data/RotatingCurve.RotatingCurve'"));
    	if (RotationCurveData.Succeeded())
    	{
    		RotationCurve = RotationCurveData.Object;
     
    	}
    
    	RotationTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeLine"));
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	PlayerAnim=Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	InitRotatingCurve();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HeadFollowing();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Trun);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
}

void APlayerCharacter::MoveForward(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void APlayerCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
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

void APlayerCharacter::HeadFollowing()
{

	//대상 전면 벡터
	FVector CameraV=FollowCamera->GetForwardVector();
	FVector PlayerV =GetActorForwardVector();
	
	PlayerV.Normalize();
	CameraV.Normalize();

	//벡터를 가지고 qutarnion 구하기
	FQuat Rot=Math::VectorA2BRotation(PlayerV,CameraV);

	if(bIsDebug)
	{
		FVector RotateVec = Rot.RotateVector(PlayerV);

		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + (PlayerV * 100), 300.0f, FLinearColor::Blue, 0.1f, 3.0f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + (CameraV * 100), 300.0f, FLinearColor::Red, 0.1f, 3.0f);
	}

	//머리 회전 값 전달
	PlayerAnim->NewRotator=Rot.Rotator();
    

	//벡터 값을 가지고 
	FVector  Temp=CameraV;
	Temp.Z=0.0f;
	float dot=  FVector::DotProduct(PlayerV,Temp);
	float Degree = UKismetMathLibrary::DegAcos(dot);
	TLOG_E(TEXT("%f"),Degree);
	

	//외적을 통해 플레이어 벡터를 중심으로 카메라가 왼쪽을 향하면 -, 오른쪽을 향하면 +를 부호로 설정해 각도 변환
	FVector OutProduct = FVector::CrossProduct(PlayerV, Temp);
	 Sign = UKismetMathLibrary::SignOfFloat(OutProduct.Z);

	//한변에 하체 및 플레이어의 방향을 회전
	if(Degree>=LimitAngle)
	{
		//AddActorWorldRotation(FRotator(0.0f,Degree*Sign,0.0f));
		PrevRotation=0.0f;
		
		RotationTimeLine->PlayFromStart();
	}
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
	PrevRotation = 0.0f;
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

