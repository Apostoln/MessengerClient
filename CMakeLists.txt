cmake_minimum_required(VERSION 3.7)
project(MessengerClient)

set(CMAKE_CXX_STANDARD 14)

add_library(asio INTERFACE )
target_include_directories(asio INTERFACE lib/asio)

add_library(easyloggingpp lib/easyloggingpp/src/easylogging++.cc)
target_include_directories(easyloggingpp INTERFACE lib/easyloggingpp/src)

add_library(argparse INTERFACE )
target_include_directories(argparse INTERFACE lib/argparse )



include_directories(src)
set(SOURCE_FILES src/main.cpp src/MessengerClient.cpp src/MessengerClient.hpp src/ProtocolMessage.hpp src/ProtocolMessage.cpp.cpp src/utils.hpp)
add_executable(MessengerClient ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} asio easyloggingpp argparse pthread)