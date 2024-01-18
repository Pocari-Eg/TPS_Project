// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "NetworkBase.h"


#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
/**
 * 
 */

enum MessageType {INVALID,ADD,REP };
 class APlayerCharacter;
class UTPSGameInstance;
class TPSGAME_API ClientThread : public FRunnable
{
#pragma region var
private:

//network
	asio::ip::tcp::endpoint ep;
	asio::io_context ios;
	asio::ip::tcp::socket sock;
	TSharedPtr<asio::io_context::work> work;

	std::string sbuf;
	std::string rbuf;
	char buf[9999];
	
	std::mutex lock;
	
	TArray<FRunnableThread*> Thread;

	FString NickName;

	APlayerCharacter* Player;
	TQueue<FString>* PlayerList;

	int32 UserCount;
#pragma endregion var


#pragma region func
public:
	ClientThread();
	~ClientThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	//  스레드 시작 함수
	void StartThreads();
	//  스레드 종료 함수
	void StopThreads();
	//send
	void Send();
	void SendHandle(const boost::system::error_code& ec);
    //recv
	void Recieve();
	void ReceiveHandle(const boost::system::error_code& ec, size_t size);
	
	void BindPlayer(FString value,APlayerCharacter* p,TQueue<FString>* TempList);

	//connect
void TryConnect();
void OnConnect(const boost::system::error_code& ec);

	//플레이어 리스트 역직렬화
	void deserializeStringArray(const std::string& serialized);
	void PacketManager(string Message);
	MessageType  TranslatePacket(string message);
	
	void AddPlayerList(string list);

	//리플리케이션
	Replication deserializeReplication(const std::string& data);
	std::vector<Replication> deserializeReplicationArray(const std::string& data);


	void GetReplicationData(string message);
private:
	string Location2String(FVector location);
	string Cutfloat(float value);
#pragma endregion func
};
