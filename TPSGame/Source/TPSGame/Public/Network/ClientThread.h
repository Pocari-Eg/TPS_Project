// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "NetworkBase.h"

#include "Async/AsyncWork.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
/**
 * 
 */

//TINIT = item+INIT 아이템 초기화
//TPICK = item+PICK
enum MessageType {INVALID,ADD,REP,TINIT,TPICK,HIT,EXIT };
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
	boost::asio::steady_timer ReplicationTimer;
	
	TArray<FRunnableThread*> Thread;

	FString NickName;

	APlayerCharacter* Player;

	int32 UserCount;


	bool bIsRunning;
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
	void Send(string Data);
	void SendHandle(const boost::system::error_code& ec);
    //recv
	void Recieve();
	void ReceiveHandle(const boost::system::error_code& ec, size_t size);
	
	void BindPlayer(FString value,APlayerCharacter* p);

	//connect
void TryConnect();
void OnConnect(const boost::system::error_code& ec);

	//플레이어 리스트 역직렬화
	void deserializeStringArray(const std::string& serialized);
	void PacketManager(string Message);
	MessageType  TranslatePacket(string message);
	
	void AddPlayerList(string list);
	void DeletePlayer(string data);
	void Initdropitem(string list);
    void PlayerPickUpItem(string data);
	
	//리플리케이션
	FReplication deserializeReplication(const std::string& data);
	std::vector<FReplication> deserializeReplicationArray(const std::string& data);

	std::string SerializeReplication(const FReplication& rep);
	
	void GetReplicationData(string message);
	void PlayerHitUpdate(string message);

	//item

	TArray<int> deserializeItemList(const std::string& str);
private:
	string Cutfloat(float value);
#pragma endregion func
};

