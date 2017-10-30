cmake_minimum_required(VERSION 3.5)
project(SIMPLE-proxy)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_custom_target(all_header_proxy SOURCES ${Headers})

link_directories(${CMAKE_SOURCE_DIR}/lib)
set(PRJ_SRCS src/main.cpp)
add_executable(proxy ${PRJ_SRCS})

set(LINK_LIBS libprotobufd.lib libprotobuf-lited.lib libprotocd.lib libzmq-v120-mt-4_0_4.lib)

target_link_libraries(proxy SIMPLE_Lib ${LINK_LIBS})
