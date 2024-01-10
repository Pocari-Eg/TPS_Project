// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWork/ClientThread.h"



ClientThread::ClientThread()
:ep(asio::ip::address::from_string(LOCALHOST_IP),SERVER_PORT),
sock(ios,ep.protocol()),
work(new asio::io_context::work(ios))
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


		TLOG_W(TEXT("WorkerThreading Start"));
		ios.run();


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
	FPlatformProcess::Sleep(0.1f);
	 ios.post(bind(&ClientThread::TryConnect, this));
}

void ClientThread::StopThreads()
{

	TLOG_E(TEXT("Stop Thread"));

	// 더 이상 새로운 비동기 작업을 받지 않도록 io_context를 멈춥니다.
	ios.stop();

	// 소켓을 닫기 전에 모든 비동기 작업이 완료될 때까지 대기합니다.
	for (int i = 0; i < 3; ++i) {
		Thread[i]->WaitForCompletion();
	}

	// 소켓을 닫고 모든 작업이 완료되었으므로 work를 리셋합니다.
	sock.close();
	work.Reset();

}

void ClientThread::Send()
{
	FPlatformProcess::Sleep(2.0f);
	sock.async_write_some(asio::buffer("TestSend"), bind(&ClientThread::SendHandle, this, _1));
}

void ClientThread::SendHandle(const boost::system::error_code& ec)
{
	if (ec)
	{
		TLOG_W(TEXT("Aysnc_read_some error"));
		Stop();
		return;
	}

	Send();
}

void ClientThread::Recieve()
{
	TLOG_W(TEXT("Recieve"));
	sock.async_read_some(asio::buffer(buf, 80), bind(&ClientThread::ReceiveHandle, this, _1, _2));
}

void ClientThread::ReceiveHandle(const boost::system::error_code& ec, size_t size)
{



	if (ec)
	{
		TLOG_W(TEXT("Aysnc_write_some error"));
		Stop();
		return;
	}

	if (size == 0)
	{
		TLOG_E(TEXT("Server wants to close this session"));
		Stop();
		return;
	}

	buf[size] = '\0';
	rbuf = buf;

	FString RecvMessage = rbuf.c_str();

	lock.lock();
     TLOG_E(TEXT("Receive data : %s"), *RecvMessage);
	lock.unlock();

	Recieve();
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


	std::string Name(TCHAR_TO_UTF8(*NickName));
	std::string LoginMessage = ":set ";
	LoginMessage += Name;
	sock.async_write_some(asio::buffer(LoginMessage), bind(&ClientThread::SendHandle, this, _1));


	ios.post(bind(&ClientThread::Send, this));
	ios.post(bind(&ClientThread::Recieve, this));
}
