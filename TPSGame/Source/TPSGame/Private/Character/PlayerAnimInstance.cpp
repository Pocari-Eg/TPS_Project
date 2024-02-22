// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Character/weapon/WeaponComponent.h"


void UPlayerAnimInstance::PlayFireMonatage()
{
	if (!Montage_IsPlaying(FireMontage)&&bIsEquip)
	{
		TLOG_E(TEXT("FIRE"));
		Montage_Play(FireMontage, 1.0f);
	}
}

void UPlayerAnimInstance::SetWalkState()
{
	if(Player!=nullptr)Player->GetFSMInstance()->ChangeState(UWalkState::GetInstance());
     bIsWalk=true;
	Player->GetWeapon()->SetWalkGrip();

	
}
	

void UPlayerAnimInstance::SetIdleState()
{
	if(Player!=nullptr)Player->GetFSMInstance()->ChangeState(UIdleState::GetInstance());
	bIsWalk=false;
	Player->GetWeapon()->SetIdleGrip();
}

void UPlayerAnimInstance::MoveDirection(float value)
{
	if(value>0.0f)bIsForward=true;
	if(value<0.0f)bIsForward=false;
}

void UPlayerAnimInstance::AnimNotify_Shoot() const
{
	OnShoot.Broadcast();
}
