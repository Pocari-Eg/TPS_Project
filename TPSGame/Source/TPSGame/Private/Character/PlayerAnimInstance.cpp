// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Character/weapon/WeaponComponent.h"


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
