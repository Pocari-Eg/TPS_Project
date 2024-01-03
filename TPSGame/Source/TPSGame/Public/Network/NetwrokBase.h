// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"

#pragma warning(disable:4005)
THIRD_PARTY_INCLUDES_START
#include <boost/asio.hpp>
THIRD_PARTY_INCLUDES_END

#include <iostream>
using namespace boost::asio;
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


