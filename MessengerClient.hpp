#ifndef MESSENGERCLIENT_MESSENGERCLIENT_HPP
#define MESSENGERCLIENT_MESSENGERCLIENT_HPP

#include <iostream>
#include <thread>
#include <cstring>
#include <asio.hpp>


using namespace asio;

class MessengerClient {
    private:
        io_service ioService;
        ip::tcp::endpoint clientEndPoint;
        ip::tcp::socket socket;
        ip::tcp::endpoint serverEndPoint;

        std::thread consoleReadThread;
        std::thread consoleWriteThread;

        static const unsigned short BUFFER_SIZE = 1024;

    public:
        MessengerClient(std::string serverIp, size_t serverPort, size_t clientPort);

        void run();
        void connect();
        void consoleRead();
        void consoleWrite();
};


#endif //MESSENGERCLIENT_MESSENGERCLIENT_HPP
