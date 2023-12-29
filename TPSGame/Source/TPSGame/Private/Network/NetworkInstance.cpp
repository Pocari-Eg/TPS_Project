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

        // ������ ����
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));

        TLOG_W(TEXT("Conncet DB SERVER"));
 

        //������ �����͸� ,�� ������ ����
        FString Data = id.ToString() + ',' + pwd.ToString() + ',' + name.ToString();

        //������ ���� fstring string���� ��ȯ
        std::string input = std::string(TCHAR_TO_UTF8(*Data));

        // ������ ������ ����
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

        // ������ ����
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));

        TLOG_W(TEXT("Conncet DB SERVER"));


        for (;;) {
            //������ �����͸� ,�� ������ ����
            FString Data = id + ',' + pwd + ',' + name;

            //������ ���� fstring string���� ��ȯ
            std::string input = std::string(TCHAR_TO_UTF8(*Data));

            // ������ ������ ����
            write(socket, buffer(input));
            TLOG_W(TEXT("send DB SERVER"));

            // �������� ���� ����
            std::vector<char> response(1024); // std::array ��� std::vector ���
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
