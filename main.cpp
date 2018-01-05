#include "MessengerClient.hpp"

static const char* SERVER_IP = "127.0.0.1";
static int SERVER_PORT = 1234;
const char message[] = "Hello, server";

int main() {
    MessengerClient client(SERVER_IP, SERVER_PORT, 7777);
    client.run(message);
    return 0;
}