// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateInterface.h"
#include "TPSGame.h"

#include "PlayerFSM.generated.h"
/**
 * 
 */
class AIreneCharacter;
UCLASS()
class UIdleState final :public UObject, public IState
{
	GENERATED_BODY()
public:
	static UIdleState* GetInstance();
	virtual void Enter(IBaseGameEntity* CurState) override;
	virtual void Execute(IBaseGameEntity* CurState) override;
	virtual void Exit(IBaseGameEntity* CurState) override;
};
UCLASS()
class UWalkState final :public UObject, public IState
{
	GENERATED_BODY()
	public:
	static UWalkState* GetInstance();
	virtual void Enter(IBaseGameEntity* CurState) override;
	virtual void Execute(IBaseGameEntity* CurState) override;
	virtual void Exit(IBaseGameEntity* CurState) override;

};
UCLASS()
class URunState final :public UObject, public IState
{
	GENERATED_BODY()
public:
	static URunState* GetInstance();
	virtual void Enter(IBaseGameEntity* CurState) override;
	virtual void Execute(IBaseGameEntity* CurState) override;
	virtual void Exit(IBaseGameEntity* CurState) override;

};






UCLASS()
class TPSGAME_API UPlayerFSM final :public UObject, public IBaseGameEntity
{
	GENERATED_BODY()
private:
	IState* StateValue;
	State CurState;
public:
	UPlayerFSM():StateValue(UIdleState::GetInstance()), CurState(State::IDLE)
	{	
	}

	virtual void Update() override;
	virtual void ChangeState(IState* NewState) override;
	virtual void ThrowState(IState* NewState) override;

	void SetPlayer(APlayerCharacter* Value);


	virtual void SetStateEnum(State Value)  override;
	State GetCurState() const { return CurState; }
	FString GetStateToString() const;
};
