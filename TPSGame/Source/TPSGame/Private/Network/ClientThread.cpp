// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWork/ClientThread.h"
#include "Character/PlayerCharacter.h"
#include "TPSGameInstance.h"

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
	for (int i = 0; i < Thread.Max(); ++i) {
		Thread[i]->WaitForCompletion();
	}

	// 소켓을 닫고 모든 작업이 완료되었으므로 work를 리셋합니다.
	sock.close();
	work.Reset();

}

void ClientThread::Send()
{
	FPlatformProcess::Sleep(0.05f);

	// if(IsValid(Player))
	// {
	// 	string Location=Location2String(Player->GetActorLocation());
	// 	sock.async_write_some(asio::buffer(Location), bind(&ClientThread::SendHandle, this, _1));
	// }
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
	


	if(string(buf).compare(":rep ")==0)
	{
		UserCount = Player->GetInstance()->GetUserCount();
		TLOG_E(TEXT("%d"), UserCount);
		int32 bufsize=UserCount*25;
		bufsize+=5;
		string temp(buf,bufsize);
		rbuf=temp;
	}
	else
	{
		rbuf = buf;
	}
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


	ios.post(bind(&ClientThread::Send, this));
	ios.post(bind(&ClientThread::Recieve, this));
}

void ClientThread::deserializeStringArray(const std::string& serialized)
{
	std::istringstream iss(serialized);


	// 배열의 크기를 먼저 읽음
	size_t arraySize;
	iss >> arraySize;

	// 각 문자열을 차례대로 읽어와 배열에 추가
	for (size_t i = 0; i < arraySize; ++i) {
		size_t strLength;
		iss >> strLength;

		std::string str;
		iss >> str;

		FString n =str.c_str();
		PlayerList->Enqueue(n);
	}
	
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
			if(UserCount>=2) GetReplicationData(Message);
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
	string temp = message.substr(0, sizeof(":add ") - 1);
	if (temp.compare(":add ") == 0)
	{
		return MessageType::ADD;
	}
	 temp = message.substr(0, sizeof(":rep ") - 1);
	if(temp.compare(":rep ")==0)
	{
		return MessageType::REP;
	}
	return MessageType::INVALID;
}

void ClientThread::AddPlayerList(string list)
{
	string temp = list.substr(sizeof(":add ") - 1, list.length());
	deserializeStringArray(temp);
}

Replication ClientThread::deserializeReplication(const std::string& data)
{
	Replication rep;

	// Pos를 역직렬화
	std::memcpy(&rep.Pos, data.data(), sizeof(rep.Pos));

	// Rot를 역직렬화
	std::memcpy(&rep.Rot, data.data() + sizeof(rep.Pos), sizeof(rep.Rot));

	// state를 역직렬화 (명시적으로 정수형으로 캐스팅)
	std::uint8_t stateValue;
	std::memcpy(&stateValue, data.data() + sizeof(rep.Pos) + sizeof(rep.Rot), sizeof(std::uint8_t));
	rep.state = static_cast<State>(stateValue);

	return rep;
}

std::vector<Replication> ClientThread::deserializeReplicationArray(const std::string& data)
{
	std::vector<Replication> repArray;

	for (std::size_t i = 0; i < data.size(); i += sizeof(repArray[0].Pos) + sizeof(repArray[0].Rot) + sizeof(std::uint8_t)) {
		// 바이트 스트림에서 각 구조체의 크기만큼 자르고 역직렬화
		std::string repData = data.substr(i, sizeof(repArray[0].Pos) + sizeof(repArray[0].Rot) + sizeof(std::uint8_t));
		repArray.push_back(deserializeReplication(repData));
	}

	return repArray;
}

void ClientThread::GetReplicationData(string message)
{

	string temp = message.substr(sizeof(":rep ") - 1, message.length());
	std::vector<Replication> restoredArray = deserializeReplicationArray(temp);


	TLOG_E(TEXT("%d"),restoredArray.size());
	for(int i=0;i<restoredArray.size();i++)
	{
		TLOG_E(TEXT("%f,%f,%f"),restoredArray[i].Pos[0],restoredArray[i].Pos[1],restoredArray[i].Pos[2]);
		TLOG_E(TEXT("%f,%f,%f"),restoredArray[i].Rot[0],restoredArray[i].Rot[1],restoredArray[i].Rot[2]);

		switch (restoredArray[i].state)
		{
		case State::IDLE:
			TLOG_E(TEXT("IDLE"));
			break;
		case State::WALK:
			TLOG_E(TEXT("WALK"));
			break;
		default:
			break;
		}

		TLOG_E(TEXT("////////////"));
	}
}

std::string ClientThread::Location2String(FVector location)
{
string str=":pos ";
	
	str+=Cutfloat(location.X)+",";
	str+=Cutfloat(location.Y)+",";
	str+=Cutfloat(location.Z);
	return str; 
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
