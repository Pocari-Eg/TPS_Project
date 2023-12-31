// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"
#include "Network/LoginManager.h"
void UTPSGameInstance::MysqlTest()
{

	LoginManager* m_login=new LoginManager();
	m_login->Test();
}
