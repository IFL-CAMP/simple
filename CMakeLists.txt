cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include include/Protobuf include/ZMQ)

file(GLOB_RECURSE Headers "*.h" "*.hpp")


set(LIB_SRCS src/Protobuf/SIMPLE.pb.cc src/Server.cpp src/Publisher.cpp src/Proxy.cpp src/Client.cpp)

add_library(SIMPLE_Lib STATIC ${LIB_SRCS})

add_subdirectory("publisher")
add_subdirectory("subscriber")
add_subdirectory("proxy")
add_subdirectory("client")
add_subdirectory("server")

