// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "UObject/Interface.h"
#include<memory>

#include "StateInterface.generated.h"

class IBaseGameEntity;
class IState;
class APlayerCharacter;



UINTERFACE()
class UState : public UInterface
{
	GENERATED_BODY()
	
};
class IState //need UState Interface
{
GENERATED_BODY()
public:
	virtual void Enter(IBaseGameEntity*) = 0; //pure virtual
	virtual void Execute(IBaseGameEntity*) = 0;
	virtual void Exit(IBaseGameEntity*) = 0;
};


UINTERFACE()
class UBaseGameEntity : public UInterface
{
	GENERATED_BODY()
	};
class IBaseGameEntity//need UBaseGameEntity Interface
{
	GENERATED_BODY()
public:
	TScriptInterface<APlayerCharacter> Player;

public:
	virtual void Update() = 0;
	virtual void ChangeState(IState* NewState) = 0;
	virtual void ThrowState(IState* NewState) = 0;

	virtual void SetStateEnum(State value) = 0;

};