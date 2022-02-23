cmake_minimum_required(VERSION 3.10)
project(P2PNetworkTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(P2PNetworkTestServer server.cpp)

add_executable(P2PNetworkTestClient client.cpp)

add_executable(delayServer delayServer.cpp)

add_executable(delayClient delayClient.cpp)
