// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/LoginManager.h"
#include "Network/NetwrokBase.h"

LoginManager::LoginManager()
{
}

LoginManager::~LoginManager()
{
}

FString LoginManager::Login(FText id, FText pwd)
{
 
        io_context io_context;

        ip::tcp::socket socket(io_context);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));
       // socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8888));
        TLOG_W(TEXT("Conncet DB SERVER"));


        LoginStruct Data;
        Data.id = ConvertArrayChar<25>(id);
        Data.pwd = ConvertArrayChar<25>(pwd);
        Data.NickName = ConvertArrayChar<25>("LOGIN");



        for (;;) {

            std::vector<char> serializedData = serialize(Data);
            boost::system::error_code ignored_error;
            write(socket, buffer(serializedData), ignored_error);



            std::vector<char> response(1024);
            size_t bytesRead = socket.read_some(buffer(response));
            std::string ServerRe = std::string(response.data(), bytesRead);

            FString signal = ServerRe.c_str();


            return signal;

        }

    return "Error";
}

FString LoginManager::Join(FText id, FText pwd, FText name)
{


         io_context io_context;
                   
        ip::tcp::socket socket(io_context);
         socket.connect(ip::tcp::endpoint(ip::address::from_string("34.64.125.122"), 8888));
         // socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8888));
           TLOG_W(TEXT("Conncet DB SERVER"));
                   

        LoginStruct Data;
        Data.id = ConvertArrayChar<25>(id);
        Data.pwd = ConvertArrayChar<25>(pwd);
        Data.NickName = ConvertArrayChar<25>(name);


       
        for (;;) {
    
            std::vector<char> serializedData =serialize(Data);
            boost::system::error_code ignored_error;
            write(socket, buffer(serializedData), ignored_error);
            
    
    
            std::vector<char> response(1024); 
            size_t bytesRead = socket.read_some(buffer(response));
            std::string ServerRe = std::string(response.data(), bytesRead);
    
            FString signal = ServerRe.c_str();
           

            return signal;

        }
     
    return "Error";
}


std::vector<char> LoginManager::serialize(const LoginStruct& data)
{
    std::vector<char> buffer(sizeof(data));
    memcpy(buffer.data(), &data, sizeof(data));
    return buffer;
}
LoginStruct LoginManager::deserialize(const std::vector<char>& buffer)
{
    LoginStruct result;
    memcpy(&result, buffer.data(), sizeof(result));
    return result;
}
