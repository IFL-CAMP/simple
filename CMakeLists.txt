cmake_minimum_required(VERSION 3.5)
project(simple)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(simple_msgs)

find_package(cppzmq REQUIRED)

include_directories(${cppzmq_INCLUDE_DIR} ${ZeroMQ_INCLUDE_DIR} ${Protobuf_INCLUDE_DIR})

# Maybe change the directory where the protobuf messages generated binaries are dropped and change this to that
include_directories(${CMAKE_CURRENT_BINARY_DIR} include ${cppzmq_INCLUDE_DIR})

file(GLOB_RECURSE Headers "*.h" "*.hpp")

set(LIB_SRCS 
  ${Headers}
	src/Server.cpp src/Publisher.cpp 
	src/Proxy.cpp src/Client.cpp 
	src/Subscriber.cpp 
	src/MSGcreator.cpp 
	src/MSGreader.cpp)

add_library(${PROJECT_NAME} ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME} simple_msgs ${cppzmq_STATIC_LIBRARY})

## TESTS

add_subdirectory(tests)

