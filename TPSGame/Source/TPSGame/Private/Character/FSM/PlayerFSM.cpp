// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/FSM/PlayerFSM.h"
#include "Character/PlayerCharacter.h"


#pragma region FSM
void UPlayerFSM::Update()
{
	StateValue->Execute(this);
}

void UPlayerFSM::ChangeState(IState* NewState)
{
	if (this != nullptr) {
		if (NewState != StateValue) {
			StateValue->Exit(this);
			StateValue = NewState;
			StateValue->Enter(this);
		//	TLOG_E(TEXT("state change to %s"), *GetStateToString());
		}
	}
}

void UPlayerFSM::ThrowState(IState* NewState)
{
	NewState->Enter(this);
	NewState->Exit(this);
}

void UPlayerFSM::SetPlayer(APlayerCharacter* Value)
{
	Player = Value;
	StateValue = UIdleState::GetInstance();
	CurState = State::IDLE;
}


void UPlayerFSM::SetStateEnum(State Value)
{
	CurState = Value;
}

FString UPlayerFSM::GetStateToString() const
{
	switch (CurState)
	{
	case State::IDLE:
		return FString("IDLE");
	case State::WALK:
		return FString("WALK");

	}

	return FString("Error");
}
#pragma endregion FSM


#pragma region WalkState
UWalkState* UWalkState::GetInstance()
{
	static UWalkState* Instance;
	if (Instance == nullptr) {
		Instance = NewObject<UWalkState>();
		Instance->AddToRoot();
	}
	return Instance;
}
void UWalkState::Enter(IBaseGameEntity* CurState)
{
	CurState->SetStateEnum(State::WALK);
}

void UWalkState::Execute(IBaseGameEntity* CurState)
{
}

void UWalkState::Exit(IBaseGameEntity* CurState)
{
}
#pragma endregion WalkState

#pragma region IdleState
UIdleState* UIdleState::GetInstance()
{
	static UIdleState* Instance;
	if (Instance == nullptr) {
		Instance = NewObject<UIdleState>();
		Instance->AddToRoot();
	}
	return Instance;
}
void UIdleState::Enter(IBaseGameEntity* CurState)
{
	CurState->SetStateEnum(State::IDLE);
	

}
void UIdleState::Execute(IBaseGameEntity* CurState)
{
}

void UIdleState::Exit(IBaseGameEntity* CurState)
{
}
#pragma endregion IdleState
