// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWork/ClientThread.h"
#include "Character/PlayerCharacter.h"
#include "TPSGameInstance.h"



ClientThread::ClientThread()
:ep(asio::ip::address::from_string(LOCALHOST_IP),SERVER_PORT),
sock(ios,ep.protocol()),
work(new asio::io_context::work(ios))
,ReplicationTimer(ios)
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
	bIsRunning=true;
}

void ClientThread::StopThreads()
{

	if(bIsRunning)
	{
		bIsRunning=false;
		TLOG_E(TEXT("Stop Thread"));

		// 더 이상 새로운 비동기 작업을 받지 않도록 io_context를 멈춥니다.
		ios.stop();

		// 소켓을 닫기 전에 모든 비동기 작업이 완료될 때까지 대기합니다.
		for (int i = 0; i < Thread.Max(); ++i) {
			Thread[i]->WaitForCompletion();
		}

		// 소켓을 닫고 모든 작업이 완료되었으므로 work를 리셋합니다.
		sock.close();
		work.Reset();


	}
}

void ClientThread::Send()
{

	if(IsValid(Player))
	{

		const FVector PlayerPos=Player->GetActorLocation();
		const float PosX=PlayerPos.X;
		const float PosY=PlayerPos.Y;
		const float PosZ=PlayerPos.Z;
		const float RotZ=Player->GetActorRotation().Yaw;
		
		FReplication temp = {PosX,PosY,PosZ,RotZ,Player->GetFSMInstance()->GetCurState()};
		
		string Replication=":rep "+SerializeReplication(temp);

		 sock.async_write_some(asio::buffer(Replication), bind(&ClientThread::SendHandle, this, _1));
	}

	ReplicationTimer.expires_at(ReplicationTimer.expires_at() + std::chrono::milliseconds(100));
	// 다음 타이머 시작
	ReplicationTimer.async_wait(boost::bind(&ClientThread::Send, this));
}

void ClientThread::Send(string Data)
{
	sock.async_write_some(asio::buffer(Data), bind(&ClientThread::SendHandle, this, _1));
}

void ClientThread::SendHandle(const boost::system::error_code& ec)
{
	if (ec)
	{
		TLOG_W(TEXT("Aysnc_read_some error"));
		Stop();
		return;
	}
	
}

void ClientThread::Recieve()
{
	sock.async_read_some(asio::buffer(buf, 9999), bind(&ClientThread::ReceiveHandle, this, _1, _2));

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
	


	UserCount=Player->GetInstance()->GetUserCount();

	rbuf = buf;
	
	PacketManager(rbuf);

	Recieve();
}

void ClientThread::BindPlayer(FString value, APlayerCharacter* p,TQueue<FString>* TempList)
{
	Player=p;
	NickName=value;
	PlayerList=TempList;
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

	ReplicationTimer.expires_from_now(std::chrono::seconds(1)); // 초기 타이머 만료 시간 설정
	ReplicationTimer.async_wait(boost::bind(&ClientThread::Send, this)); // 타이머 시작
	ios.post(bind(&ClientThread::Recieve, this));
}

void ClientThread::deserializeStringArray(const std::string& serialized)
{
	std::istringstream iss(serialized);


	// 배열의 크기를 먼저 읽음
	size_t arraySize;
	iss >> arraySize;

	Player->SetAddPlayerCount(arraySize);
	// 각 문자열을 차례대로 읽어와 배열에 추가
	for (size_t i = 0; i < arraySize; ++i) {
		size_t strLength;
		iss >> strLength;

		std::string str;
		iss >> str;

		FString n = str.c_str();
		PlayerList->Enqueue(n);
		
	}
	TLOG_W(TEXT("Add Player List"));
	
}

void ClientThread::PacketManager(string Message)
{
	// :~ 라는 특수(?)메세지를 보내왔을 경우 처리
	if (Message[0] == ':')
	{
		MessageType code = TranslatePacket(Message);

		switch (code)
		{
		case MessageType::ADD:
			AddPlayerList(Message);
			break;
		case MessageType::REP:
			
			if(UserCount>=2)
			{
				GetReplicationData(Message);
			}
			break;

		case MessageType::HIT:
			PlayerHitUpdate(Message);
			break;
		case MessageType::EXIT:
			DeletePlayer(Message);
			break;
		case MessageType::INVALID:
            TLOG_E(TEXT("Unsupported message command."));
			break;
		}
	}
	else  // :~ 라는 특수메세지가 아니고 그냥 채팅일 경우
	{
		FString msg=Message.c_str();
		lock.lock();
		TLOG_E(TEXT("%s"),*msg);
		lock.unlock();
	}
}

MessageType ClientThread::TranslatePacket(string message)
{

	//add
	string temp = message.substr(0, sizeof(":add ") - 1);
	if (temp.compare(":add ") == 0)
	{
		return MessageType::ADD;
	}
	//rep
	 temp = message.substr(0, sizeof(":rep ") - 1);
	if(temp.compare(":rep ")==0)
	{
		return MessageType::REP;
	}
	//hit
	temp = message.substr(0, sizeof(":hit ") - 1);
	if(temp.compare(":hit ")==0)
	
	{
		return MessageType::HIT;
	}

	//death
	temp = message.substr(0, sizeof(":exit ") - 1);
	if(temp.compare(":exit ")==0)
	{
		return MessageType::EXIT;
	}
	return MessageType::INVALID;
}

void ClientThread::AddPlayerList(string list)
{
	string temp = list.substr(sizeof(":add ") - 1, list.length());
	deserializeStringArray(temp);
}

void ClientThread::DeletePlayer(string data)
{
	string temp = data.substr(sizeof(":exit ") - 1, data.length());

	FString name =temp.c_str();
	Player->GetInstance()->DeletePlayer(name);
}

FReplication ClientThread::deserializeReplication(const std::string& data)
{
	FReplication replication;
	std::stringstream ss(data);

	// PosX, PosY, PosZ, RotZ를 문자열에서 읽어오기
	ss >> replication.PosX >> replication.PosY >> replication.PosZ >> replication.RotZ;

	// State 열거체 값을 int로 읽어와 다시 열거체 값으로 변환
	int stateValue;
	ss >> stateValue;
	replication.state = static_cast<State>(stateValue);

	return replication;
}

std::vector<FReplication> ClientThread::deserializeReplicationArray(const std::string& data)
{
	std::vector<FReplication> replicationVector;
	std::stringstream ss(data);
	std::string line;

	while (std::getline(ss, line)) {
		FReplication replication = deserializeReplication(line); // 각 줄의 문자열을 역직렬화하여 FReplication 객체에 저장
		replicationVector.push_back(replication);
	}

	return replicationVector;
}

std::string ClientThread::SerializeReplication(const FReplication& rep)
{
	std::stringstream ss;

	// PosX, PosY, PosZ, RotZ를 문자열로 쓰기
	ss << rep.PosX << ' ' << rep.PosY << ' ' << rep.PosZ << ' ' << rep.RotZ << ' ';

	// State 열거체 값을 int로 변환하여 쓰기
	ss << static_cast<int>(rep.state);

	return ss.str();
}

void ClientThread::GetReplicationData(string message)
{

	string temp = message.substr(sizeof(":rep ") - 1, message.length());
	std::vector<FReplication> restoredVector = deserializeReplicationArray(temp);
	
	
	Player->GetInstance()->UpdateUserPos(restoredVector);
	
	
}

void ClientThread::PlayerHitUpdate(string message)
{

	string temp = message.substr(sizeof(":hit ") - 1, message.length());
	
	// 메시지 큐나 델리게이트를 통해 메인 스레드로 메시지 전달
	FString UEMessage=temp.c_str();
	TGraphTask<FUpdateUITask>::CreateTask(nullptr).ConstructAndDispatchWhenReady(Player, UEMessage);	
}

string ClientThread::Cutfloat(float value)
{
	std::string stringNumber = std::to_string(value);
	// 찾아낸 소수점의 위치
	size_t dotPosition = stringNumber.find('.');
    
	// 소수점 이하 1자리까지 자르기
	if (dotPosition != std::string::npos && dotPosition + 2 < stringNumber.length()) {
		stringNumber.erase(dotPosition + 2, stringNumber.length() - dotPosition - 2);
	}

	return stringNumber;
}

FUpdateUITask::FUpdateUITask(APlayerCharacter* InPlayer, const FString& InMessage)
{
	Player=InPlayer;
	Message=InMessage;
}

void FUpdateUITask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	// 메인 스레드에서 UI 업데이트 호출
	if (Player.IsValid())
	{
		Player->UpdateUIOnMainThread(Message);
	}
}

