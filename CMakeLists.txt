cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include include/Protobuf include/ZMQ)

set(LIB_SRCS include/Protobuf/SIMPLE.pb.cc)

add_library(SIMPLE_Lib ${LIB_SRCS})

add_subdirectory("publisher")
add_subdirectory("subscriber")

