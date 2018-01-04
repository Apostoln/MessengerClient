#include <iostream>
#include <cstring>
#include <asio.hpp>

using namespace asio;

int main() {
    static const char* SERVER_IP = "127.0.0.1";
    static int SERVER_PORT = 1234;
    const char message[] = "Hello, server";
    io_service ioService;
    ip::tcp::endpoint serverEndPoint(ip::address::from_string(SERVER_IP), SERVER_PORT);
    ip::tcp::socket socket(ioService);
    socket.connect(serverEndPoint);
    socket.write_some(buffer(message));
    std::cout << "Send " << socket.remote_endpoint() << " >(" << strlen(message) << ") " << message << std::endl;

    const int BUFFER_SIZE = 1024;
    char data[BUFFER_SIZE] = {0};
    size_t messageLength = socket.read_some(buffer(data));
    std::cout << "Echo " << socket.remote_endpoint() << " <(" << strlen(data) << ") " << data << std::endl;
    socket.close();
    return 0;
}