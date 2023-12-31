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
};

struct RegiStruct
{
    std::array<char, 25> id;
    std::array<char, 25> pwd;
    std::array<char, 50> NickName;
};


template<size_t arraySize>
static std::array<char, arraySize> ConvertArrayChar( FText Original)
{

    std::array<char, arraySize> resultArray;
    // FText를 FString으로 변환
    FString myFString = Original.ToString();

    // FString을 std::string으로 변환
    std::string myStdString(TCHAR_TO_UTF8(*myFString));
    
    std::copy(myStdString.begin(), myStdString.end(), resultArray.begin());

    return resultArray;

}
template<size_t arraySize>
static std::array<char, arraySize> ConvertArrayChar( FString Original)
{
    std::array<char, arraySize> resultArray;
    // FString을 std::string으로 변환
    std::string myStdString(TCHAR_TO_UTF8(*Original));

    std::copy(myStdString.begin(), myStdString.end(), resultArray.begin());

    return resultArray;
}
