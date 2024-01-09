// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWork/ClientThread.h"


ClientThread::ClientThread()
:ep(ip::address::from_string(LOCALHOST_IP),SERVER_PORT),
sock(ios,ep.protocol()),
work(new io_context::work(ios))
{}

ClientThread::~ClientThread()
{
}

bool ClientThread::Init()
{
	return true;
}

uint32 ClientThread::Run()
{
	// 비동기적으로 데이터를 수신하는 로직을 구현합니다.
	if (!Stopping)
	{
		TLOG_W(TEXT("WorkerThreading Start"));
		ios.run();
		TLOG_W(TEXT("WorkerThreading End"));
	}

	return 0;
}


void ClientThread::Stop()
{

	StopThreads();
}

void ClientThread::Exit()
{
	FRunnable::Exit();
}

void ClientThread::StartThreads()
{
	
	 TLOG_W(TEXT("Set Threads"));
	Thread.Init(nullptr,3);

	for(int i=0;i<3;i++)
	{
		Thread[i] = FRunnableThread::Create(this, *FString::Printf(TEXT("WorkerThreading_%d"), i), 0, TPri_BelowNormal);
		TLOG_W(TEXT("WorkingThread created"));
	}
	 FPlatformProcess::Sleep(0.1f);  // 작업 간격 동안 스레드를 잠시 대기
	Stopping = false;
	 ios.post(bind(&ClientThread::TryConnect, this));
}

void ClientThread::StopThreads()
{

	Stopping = true;
	
}



void ClientThread::TryConnect()
{
	TLOG_W(TEXT("Try Connect"));
	sock.async_connect(ep, boost::bind(&ClientThread::OnConnect, this, _1));
}

void ClientThread::OnConnect(const boost::system::error_code& ec)
{
	TLOG_W(TEXT("On_connect"));
	if (ec)
	{
		TLOG_W(TEXT("Conncect_Fail"));
		sock.close();
		work.Reset();
		return;
	}
	TLOG_W(TEXT("Conncect_Success"));
	//ios.post(bind(&Client::Send, this));
	///ios.post(bind(&Client::Recieve, this));
}
