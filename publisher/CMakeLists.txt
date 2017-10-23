cmake_minimum_required(VERSION 3.5)
project(SIMPLE-publisher)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include)

set(LIB_SRCS src/Publisher.cpp)

file(GLOB_RECURSE Headers "*.h" "*.hpp")
add_custom_target(all_header_publisher SOURCES ${Headers})

add_library(SIMPLE_Lib_publisher ${LIB_SRCS})

link_directories(${CMAKE_SOURCE_DIR}/lib)
set(PRJ_SRCS src/main.cpp)
add_executable(publisher ${PRJ_SRCS})

set(LINK_LIBS libprotobufd.lib libprotobuf-lited.lib libprotocd.lib libzmq-v120-mt-4_0_4.lib)

target_link_libraries(publisher SIMPLE_Lib SIMPLE_Lib_publisher ${LINK_LIBS})
