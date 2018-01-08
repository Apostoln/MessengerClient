#include <argparse.hpp>

#include "MessengerClient.hpp"
#include "utils.hpp"

static const char* SERVER_IP = "127.0.0.1";
static int SERVER_PORT = 1234;

int main(int argc, const char* argv[]) {
    ArgumentParser parser;
    parser.addArgument("-s", "--serverIp", 1);
    parser.addArgument("-p", "--serverPort", 1);
    parser.addArgument("-m", "--myPort", 1);
    parser.addArgument("-l", "--logLevel", 1);
    parser.addArgument("-c", "--cout", 1);
    parser.addArgument("-f", "--fileLogger", 1);
    parser.parse(argc, argv);

    auto serverIp = parser.retrieve<std::string>("serverIp");
    auto serverPort = std::stoi(parser.retrieve<std::string>("serverPort"));
    auto clientPort = std::stoi(parser.retrieve<std::string>("myPort"));
    auto logLevel = getLogLevel(parser.retrieve<std::string>("logLevel"));
    auto isConsoleOut = !parser.retrieve<std::string>("cout").empty();
    auto loggingFile = parser.retrieve<std::string>("fileLogger");


    MessengerClient client(serverIp, serverPort, clientPort);
    client.run();
    return 0;
}