#ifndef MESSENGERCLIENT_MESSENGERCLIENT_HPP
#define MESSENGERCLIENT_MESSENGERCLIENT_HPP

#include <iostream>
#include <cstring>
#include <asio.hpp>

using namespace asio;

class MessengerClient {
    private:
        io_service ioService;
        ip::tcp::endpoint clientEndPoint;
        ip::tcp::socket socket;
        ip::tcp::endpoint serverEndPoint;

    public:
        MessengerClient(std::string serverIp, size_t serverPort, size_t clientPort);

        void run(const std::string& message);
};


#endif //MESSENGERCLIENT_MESSENGERCLIENT_HPP
