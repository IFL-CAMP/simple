cmake_minimum_required(VERSION 3.5)
project(SIMPLE-subscriber)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include ${CMAKE_SOURCE_DIR}/include${CMAKE_SOURCE_DIR}/include/Protobuf ${CMAKE_SOURCE_DIR}/include/ZMQ)

set(LIB_SRCS src/Subscriber.cpp)

file(GLOB_RECURSE Headers "*.h" "*.hpp")
add_custom_target(all_header_subscriber SOURCES ${Headers})

add_library(SIMPLE_Lib_subscriber ${LIB_SRCS})

link_directories(${CMAKE_SOURCE_DIR}/lib)
set(PRJ_SRCS src/main.cpp)
add_executable(subscriber ${PRJ_SRCS})

set(LINK_LIBS libprotobufd.lib libprotobuf-lited.lib libprotocd.lib libzmq-v120-mt-4_0_4.lib)

target_link_libraries(subscriber SIMPLE_Lib SIMPLE_Lib_subscriber ${LINK_LIBS})
