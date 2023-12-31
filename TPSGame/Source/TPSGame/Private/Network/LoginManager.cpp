// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/LoginManager.h"
#include "Network/NetwrokBase.h"

LoginManager::LoginManager()
{
}

LoginManager::~LoginManager()
{
}

bool LoginManager::Login(FText id, FText pwd)
{
 
    return false;
}

bool LoginManager::Register(FText id, FText pwd, FText name)
{

    try {
        io_context io_context;

       
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));
       

        TLOG_W(TEXT("Conncet DB SERVER"));
 

        FString Data = id.ToString() + ',' + pwd.ToString() + ',' + name.ToString();


        std::string input = std::string(TCHAR_TO_UTF8(*Data));

       
        write(socket, buffer(input));

        return true;
    }
    catch (std::exception& e) {
        FString msg = e.what();
        return false;
    }

    return false;
}

void LoginManager::Test()
{

    try {

        io_context io_context;
        
        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));
        //socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8888));
        TLOG_W(TEXT("Conncet DB SERVER"));
         
        FString m_id = "test";
        FString m_pwd = "1111";
        FString m_Nick = "Pocari";


        RegiStruct Data;

        Data.id = ConvertArrayChar<25>(m_id);
        Data.pwd = ConvertArrayChar<25>(m_pwd);
        Data.NickName = ConvertArrayChar<50>(m_Nick);

   
       
        for (;;) {

            
            std::vector<char> serializedData =Regi_serialize(Data);
            boost::system::error_code ignored_error;
            write(socket, buffer(serializedData), ignored_error);
            


            std::vector<char> response(1024); 
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

std::vector<char> LoginManager::Login_serialize(const LoginStruct& data)
{
    std::vector<char> buffer(sizeof(data));
    memcpy(buffer.data(), &data, sizeof(data));
    return buffer;
}
LoginStruct LoginManager::Login_deserialize(const std::vector<char>& buffer)
{
    LoginStruct result;
    memcpy(&result, buffer.data(), sizeof(result));
    return result;
}
std::vector<char> LoginManager::Regi_serialize(const RegiStruct& data)
{
    std::vector<char> buffer(sizeof(data));
    memcpy(buffer.data(), &data, sizeof(data));
    return buffer;
}
RegiStruct LoginManager::Regi_deserialize(const std::vector<char>& buffer)
{
    RegiStruct result;
    memcpy(&result, buffer.data(), sizeof(result));
    return result;
}
