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
            std::cout << "proto" << std::endl;
            handleProtocol(fromString(data));
        }
        else {
            std::cout << "msg" << std::endl;
            socket.write_some(buffer(data));
            std::cout << socket.remote_endpoint() << " > " << data << std::endl;
        }
    }
}

void MessengerClient::consoleWrite() {
    while(true) {
        char data[BUFFER_SIZE] = {0};
        std::lock_guard<std::mutex> closingGuard(socketReadMutex);
        if (socket.available()) {
            size_t messageLength = socket.read_some(buffer(data));

            if (0 != messageLength) {
                std::cout << socket.remote_endpoint() << " < " << data << std::endl;
            }
            if (isProtocolMessage(data)) {
                if (ProtocolMessage::UNAUTH == fromString(data) ) {
                    unauth();
                }
            }
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
    std::cout << "closeConnection" << std::endl;
    std::lock_guard<std::mutex> closingGuard(socketReadMutex);
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
        case ProtocolMessage::REG: {
            registration();
            break;
        }
        case ProtocolMessage::LOGIN: {
            login();
            break;
        }
        default: {
            break;
        }
    }
}

void MessengerClient::registration() {
    std::cout << "registration" << std::endl;
    std::lock_guard<std::mutex> closingGuard(socketReadMutex);
    std::string message = protocolString[ProtocolMessage::REG];

    socket.write_some(buffer(message));
    std::cout << socket.remote_endpoint() << " >> " << message << std::endl;

    {
        char data[BUFFER_SIZE] = {0};
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint() << " << " << data << std::endl;
            if (ProtocolMessage::OK == data) {
                std::cout << "Registration request accepted, enter login password" << std::endl;
            } else {
                std::exit(2);
            }
        }

        char dataLoginPass[BUFFER_SIZE] = {0};
        std::cin.getline(dataLoginPass, BUFFER_SIZE);
        socket.write_some(buffer(dataLoginPass));
        std::cout << socket.remote_endpoint() << " > " << dataLoginPass << std::endl;
    }

    {
        char data[BUFFER_SIZE] = {0};
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint() << " << " << data << std::endl;
            if (ProtocolMessage::OK == data) {
                std::cout << "Registration successful" << std::endl;
            } else {
                std::exit(2);
            }
        }
    }
}

void MessengerClient::login() {
    std::cout << "login" << std::endl;
    std::lock_guard<std::mutex> closingGuard(socketReadMutex);
    std::string message = protocolString[ProtocolMessage::LOGIN];

    socket.write_some(buffer(message));
    std::cout << socket.remote_endpoint() << " >> " << message << std::endl;

    {
        char data[BUFFER_SIZE] = {0};
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint() << " << " << data << std::endl;
            if (ProtocolMessage::OK == data) {
                std::cout << "Login request accepted, enter login password" << std::endl;
            } else {
                std::exit(2);
            }
        }

        char dataLoginPass[BUFFER_SIZE] = {0};
        std::cin.getline(dataLoginPass, BUFFER_SIZE);
        socket.write_some(buffer(dataLoginPass));
        std::cout << socket.remote_endpoint() << " > " << dataLoginPass << std::endl;
    }

    {
        char data[BUFFER_SIZE] = {0};
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint() << " << " << data << std::endl;
            if (ProtocolMessage::OK == data) {
                std::cout << "Login successful" << std::endl;
            } else {
                std::cout << "Login failed, try again" << std::endl;
                return;
            }
        }
    }
}

void MessengerClient::unauth() {
    std::cout << "You are not authorized. Please login or register" << std::endl;
}