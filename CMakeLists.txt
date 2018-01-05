cmake_minimum_required(VERSION 3.7)
project(MessengerClient)

set(CMAKE_CXX_STANDARD 14)

add_library(asio INTERFACE )
target_include_directories(asio INTERFACE lib/asio)

set(SOURCE_FILES main.cpp MessengerClient.cpp MessengerClient.hpp ProtocolMessage.hpp)
add_executable(MessengerClient ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} asio pthread)