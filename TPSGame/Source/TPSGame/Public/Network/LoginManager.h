// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include <vector>

#include "NetworkBase.h"

struct LoginStruct;


using namespace boost::asio;
class TPSGAME_API LoginManager
{
public:
	LoginManager();
	~LoginManager();

	  FString Login(FText id,FText pwd);
	  FString Join(FText id,FText pwd,FText name);

private:


	std::vector<char>serialize(const LoginStruct& data);
	LoginStruct deserialize(const std::vector<char>& buffer);


template<size_t arraySize>
 std::array<char, arraySize> ConvertArrayChar( FText Original)
	{

		std::array<char, arraySize> resultArray;
		resultArray.fill('\0');
		FString myFString = Original.ToString();

		// FString�� std::string���� ��ȯ
		std::string myStdString(TCHAR_TO_UTF8(*myFString));
    
		std::copy(myStdString.begin(), myStdString.end(), resultArray.begin());

		return resultArray;

	}
	template<size_t arraySize>
	std::array<char, arraySize> ConvertArrayChar( FString Original)
	{
		std::array<char, arraySize> resultArray;
		resultArray.fill('\0');
		std::string myStdString(TCHAR_TO_UTF8(*Original));

		std::copy(myStdString.begin(), myStdString.end(), resultArray.begin());

		return resultArray;
	}

};
