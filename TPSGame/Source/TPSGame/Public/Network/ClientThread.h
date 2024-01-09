// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "NetworkBase.h"

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
/**
 * 
 */
class AsyncSend;
class AsyncRecv;

 
class TPSGAME_API ClientThread : public FRunnable
{
#pragma region var
private:

//thread
	FRunnableThread* RecvThread;
	FRunnableThread* SendThread;
	TSharedPtr<class AsyncRecv> RecvRunnable;
	TSharedPtr<class AsyncSend> SendRunnable;
	
	bool Stopping;	
//network
	ip::tcp::endpoint ep;
	io_context ios;
	ip::tcp::socket sock;
	TSharedPtr<io_context::work> work;

	TArray<FRunnableThread*> Thread;
#pragma endregion var


#pragma region func
public:
	ClientThread();
	~ClientThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	// 네트워크 스레드 시작 함수
	void StartThreads();

	// 네트워크 스레드 종료 함수
	void StopThreads();
	
	
void TryConnect();
void OnConnect(const boost::system::error_code& ec);
#pragma endregion func
};
