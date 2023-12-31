// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include <vector>

struct RegiStruct;
struct LoginStruct;


class TPSGAME_API LoginManager
{
public:
	LoginManager();
	~LoginManager();

	bool Login(FText id,FText pwd);
	bool Register(FText id,FText pwd,FText name);

	 void Test();
private:


	//직렬화
	std::vector<char> Login_serialize(const LoginStruct& data);
	//역직렬화
	LoginStruct Login_deserialize(const std::vector<char>& buffer);
	
	std::vector<char> Regi_serialize(const RegiStruct& data);
	RegiStruct Regi_deserialize(const std::vector<char>& buffer);
};
