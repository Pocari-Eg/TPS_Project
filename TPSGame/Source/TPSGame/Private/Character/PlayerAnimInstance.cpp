// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Character/weapon/WeaponComponent.h"


void UPlayerAnimInstance::PlayFireMonatage()
{
	if (!Montage_IsPlaying(FireMontage)&&bIsEquip&&!bIsRun&&!Player->bIsCameraControl)
	{
		if(Player->bIsDebug)TLOG_W(TEXT("FIRE"));
		Montage_Play(FireMontage, 1.0f);
	}
}

void UPlayerAnimInstance::SetWalkState()
{
	if(Player!=nullptr)Player->GetFSMInstance()->ChangeState(UWalkState::GetInstance());
	bIsRun=false;
     bIsWalk=true;
}
	

void UPlayerAnimInstance::SetIdleState()
{
	if(Player!=nullptr)Player->GetFSMInstance()->ChangeState(UIdleState::GetInstance());
	bIsRun=false;
	bIsWalk=false;
}

void UPlayerAnimInstance::SetRunState()
{
	if(Player!=nullptr)Player->GetFSMInstance()->ChangeState(URunState::GetInstance());
	bIsRun=true;
	bIsWalk=true;

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
