#include <iostream>
#include <asio.hpp>

using namespace asio;

int main() {
    static const char* SERVER_IP = "127.0.0.1";
    static int SERVER_PORT = 1234;
    io_service ioService;
    ip::tcp::endpoint serverEndPoint(ip::address::from_string(SERVER_IP), SERVER_PORT);
    ip::tcp::socket socket(ioService);
    socket.connect(serverEndPoint);
    socket.write_some(buffer("Hello"));
    socket.close();
    return 0;
}