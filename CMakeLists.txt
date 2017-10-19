cmake_minimum_required(VERSION 3.5)
project(SIMPLE)

include_directories(include include/Protobuf include/ZMQ)

set(LIB_SRCS include/Protobuf/SIMPLE.pb.cc src/Publisher.cpp)

file(GLOB_RECURSE Headers "*.h" "*.hpp")
add_custom_target(all_header SOURCES ${Headers})

add_library(SIMPLE_Lib ${LIB_SRCS})

link_directories(lib)
set(PRJ_SRCS src/main.cpp)
add_executable(publish ${PRJ_SRCS})

set(LINK_LIBS libprotobufd.lib libprotobuf-lited.lib libprotocd.lib libzmq-v120-mt-4_0_4.lib)

target_link_libraries(publish SIMPLE_Lib ${LINK_LIBS})
