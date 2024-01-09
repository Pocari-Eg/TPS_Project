// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "NetworkBase.h"
#include "HAL/Runnable.h"
/**
 * 
 */
class TPSGAME_API AsyncSend : public FRunnable
{
public:
	AsyncSend();
	~AsyncSend();
};
