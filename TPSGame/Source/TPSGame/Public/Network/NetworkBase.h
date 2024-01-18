// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include<mutex>
#pragma warning(disable:4005)
THIRD_PARTY_INCLUDES_START
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp> 
THIRD_PARTY_INCLUDES_END



#include <iostream>
#include <string>
using namespace boost::placeholders;
using namespace boost;
using namespace std;
/**
 * 
 */

struct LoginStruct
{
    std::array<char, 25> id;
    std::array<char, 25> pwd;
    std::array<char, 25> NickName;
};


enum State {
	IDLE = 0,
	WALK
};

struct Replication {
	float Pos[3];
	float Rot[3];
	State state;
};
