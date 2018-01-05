#include "MessengerClient.hpp"

MessengerClient::MessengerClient(std::string serverIp, size_t serverPort, size_t clientPort)
    : serverEndPoint(ip::address::from_string(serverIp), serverPort),
      clientEndPoint(ip::tcp::v4(), clientPort),
      socket(ioService, clientEndPoint)
{}

void MessengerClient::run(const std::string& message) {
    socket.connect(serverEndPoint);
    socket.write_some(buffer(message.c_str(), message.length()));
    std::cout << "Send " << socket.remote_endpoint() << " >(" << message.length() << ") " << message << std::endl;

    while(true) {
        const int BUFFER_SIZE = 1024;
        char data[BUFFER_SIZE] = {0};
        size_t messageLength = socket.read_some(buffer(data));
        std::cout << "Echo " << socket.remote_endpoint() << " <(" << strlen(data) << ") " << data << std::endl;
        //socket.close();
    }
}