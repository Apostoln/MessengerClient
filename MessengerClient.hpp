#ifndef MESSENGERCLIENT_MESSENGERCLIENT_HPP
#define MESSENGERCLIENT_MESSENGERCLIENT_HPP

#include <iostream>
#include <thread>
#include <cstring>
#include <asio.hpp>
#include <mutex>


using namespace asio;

enum class ProtocolMessage;

class MessengerClient {
    private:
        io_service ioService;
        ip::tcp::endpoint clientEndPoint;
        ip::tcp::socket socket;
        ip::tcp::endpoint serverEndPoint;

        std::thread consoleReadThread;
        std::thread consoleWriteThread;

        std::mutex socketReadMutex;
        std::mutex socketWriteMutex;

        static const unsigned short BUFFER_SIZE = 1024;

    public:
        MessengerClient(std::string serverIp, size_t serverPort, size_t clientPort);

        void run();
        void connect();
        void consoleRead();
        void consoleWrite();

        bool isProtocolMessage(const char*);

        void closeConnection();

        void handleProtocol(ProtocolMessage);

        void registration();
        void login();
        void unauth();
};


#endif //MESSENGERCLIENT_MESSENGERCLIENT_HPP
