#include <string>

#include "MessengerClient.hpp"
#include "ProtocolMessage.hpp"

MessengerClient::MessengerClient(std::string serverIp, size_t serverPort, size_t clientPort)
    : serverEndPoint(ip::address::from_string(serverIp), serverPort),
      clientEndPoint(ip::tcp::v4(), clientPort),
      socket(ioService, clientEndPoint)
{}

void MessengerClient::run() {
    connect();

    consoleReadThread = std::thread(&MessengerClient::consoleRead, this);
    consoleWriteThread = std::thread(&MessengerClient::consoleWrite, this);
    consoleReadThread.join();
    consoleWriteThread.join();
}

void MessengerClient::connect() {
    std::string startMessage = protocolString[ProtocolMessage::START];
    socket.connect(serverEndPoint);
    socket.write_some(buffer(startMessage.c_str(), startMessage.length()));
    std::cout << socket.remote_endpoint() << " >> " << startMessage << std::endl;


    char data[BUFFER_SIZE] = {0};
    size_t messageLength = socket.read_some(buffer(data));
    if (0 != messageLength) {
        std::cout << socket.remote_endpoint() << " << " << data << std::endl;
        if (ProtocolMessage::OK == data) {
            std::cout << "Connection successful." << std::endl;
        } else {
            std::cout << "Connection failed." << std::endl;
            std::exit(1);
        }
    }
}

void MessengerClient::consoleRead() {
    while (true) {

        char data[BUFFER_SIZE] = {0};
        std::cin.getline(data, BUFFER_SIZE);
        if(isProtocolMessage(data)) {
            handleProtocol(fromString(data));
        }
        else {
            socket.write_some(buffer(data));
            std::cout << socket.remote_endpoint() << " > " << data << std::endl;
        }
    }
}

void MessengerClient::consoleWrite() {
    while(true) {
        char data[BUFFER_SIZE] = {0};
        //std::lock_guard<std::mutex> closingGuard(socketReadMutex);
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint() << " < " << data << std::endl;
        }
    }
}

bool MessengerClient::isProtocolMessage(const char* msg) {
    if ( '#' == msg[0]) {
        return true;
    }
    return false;
}

void MessengerClient::closeConnection() {
    //std::lock_guard<std::mutex> closingGuard(socketReadMutex);
    std::string message = protocolString[ProtocolMessage::CANCEL];
    socket.write_some(buffer(message));
    std::cout << socket.remote_endpoint() << " >> " << message << std::endl;

    char data[BUFFER_SIZE] = {0};
    size_t messageLength = socket.read_some(buffer(data));
    if (0 != messageLength) {
        std::cout << socket.remote_endpoint() << " << " << data << std::endl;
        if (ProtocolMessage::OK == data) {
            std::cout << "Connection closed successful." << std::endl;
            std::exit(0);
        } else {
            std::cout << "Connection closed failing." << std::endl;
            std::exit(1);
        }
    }

}

void MessengerClient::handleProtocol(ProtocolMessage msg) {
    switch(msg) {
        case ProtocolMessage::CANCEL: {
            closeConnection();
            break;
        }
        default: {
            break;
        }
    }
}
