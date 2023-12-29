// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkInstance.h"

NetworkInstance::NetworkInstance()
{
}

NetworkInstance::~NetworkInstance()
{
}

bool NetworkInstance::Login(FText id, FText pwd)
{
 
    return false;
}

bool NetworkInstance::Register(FText id, FText pwd, FText name)
{

    try {
        io_context io_context;

        // 서버에 연결
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));

        TLOG_W(TEXT("Conncet DB SERVER"));
 

        //각각의 데이터를 ,로 구분해 저장
        FString Data = id.ToString() + ',' + pwd.ToString() + ',' + name.ToString();

        //전송을 위해 fstring string으로 변환
        std::string input = std::string(TCHAR_TO_UTF8(*Data));

        // 서버로 데이터 전송
        write(socket, buffer(input));

        return true;
    }
    catch (std::exception& e) {
        FString msg = e.what();
        return false;
    }

    return false;
}

void NetworkInstance::Test()
{

    TLOG_W(TEXT("Try Test"));
    try {
        FString id, pwd, name;
        id = "Test";
        pwd = "1111";
        name = "Pocari";

        io_context io_context;

        // 서버에 연결
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));

        TLOG_W(TEXT("Conncet DB SERVER"));


        for (;;) {
            //각각의 데이터를 ,로 구분해 저장
            FString Data = id + ',' + pwd + ',' + name;

            //전송을 위해 fstring string으로 변환
            std::string input = std::string(TCHAR_TO_UTF8(*Data));

            // 서버로 데이터 전송
            write(socket, buffer(input));
            TLOG_W(TEXT("send DB SERVER"));

            // 서버에서 응답 받음
            std::vector<char> response(1024); // std::array 대신 std::vector 사용
            size_t bytesRead = socket.read_some(buffer(response));

            std::string ServerRe = std::string(response.data(), bytesRead);

            FString signal = ServerRe.c_str();
            TLOG_W(TEXT("%s"), *signal);
           
            if (signal.Equals("Success"))
            {
                TLOG_E(TEXT("SUCCESS"));
                return;
            }
        }
      
    }
    catch (std::exception& e) {

        FString msg = e.what();
        TLOG_W(TEXT("%s"), *msg);
    }

}
