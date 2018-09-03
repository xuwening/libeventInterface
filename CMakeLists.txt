
cmake_minimum_required(VERSION 2.6)

project(libevent_test)

message(STATUS "binary dir: " ${PROJECT_BINARY_DIR})
message(STATUS "source dir: " ${PROJECT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -g -ggdb -Wall")
#set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb -Wall")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

INCLUDE_DIRECTORIES(./)

set(SRC_LIST HttpContainer.cpp HttpRequest.cpp HttpServer.cpp HttpServerMsgHandler.cpp MessageHandler.cpp TcpClientThread.cpp TcpServer.cpp TcpServerMsgHandler.cpp Threadable.cpp TimeoutServer.cpp AbstractServer.cpp ConnectionManager.cpp Configure.cpp LibeventLog.cpp InputHandler.cpp)

add_library(eventInterface ${SRC_LIST})

ADD_EXECUTABLE(libevent_test libevent_test.cpp  ${SRC_LIST})
TARGET_LINK_LIBRARIES(libevent_test event)







