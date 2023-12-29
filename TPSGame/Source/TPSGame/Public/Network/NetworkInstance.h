// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"

#pragma warning(disable:4005)
THIRD_PARTY_INCLUDES_START
#include <boost/asio.hpp>
THIRD_PARTY_INCLUDES_END

#include <iostream>
using namespace boost::asio;
/**
 * 
 */
class TPSGAME_API NetworkInstance
{
public:
	NetworkInstance();
	~NetworkInstance();

	bool Login(FText id,FText pwd);
	bool Register(FText id,FText pwd,FText name);

	static void Test();

};
