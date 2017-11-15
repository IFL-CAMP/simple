cmake_minimum_required(VERSION 3.5)
project(simple)

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
  list(APPEND CMAKE_CXX_FLAGS_RELEASE " /MT")
  list(APPEND CMAKE_CXX_FLAGS_DEBUG " /MTd")
endif(WIN32)

# Build simple_msgs
add_subdirectory(simple_msgs)

# From here on there is only stuff related to the lib "simple"
if (WIN32)
  set(ZeroMQ_INCLUDE_DIR "" CACHE FILEPATH "Include directory for ZeroMQ")
  set(cppzmq_LIBRARY "" CACHE FILEPATH "The ZeroMQ library file")
  set(cppzmq_INCLUDE_DIR "" CACHE FILEPATH "Include directory for cppzmq")
else()
  find_package(cppzmq REQUIRED)
endif(WIN32)

if ((NOT ((ZeroMQ_INCLUDE_DIR) AND (cppzmq_LIBRARY) AND (cppzmq_INCLUDE_DIR))))
  message(FATAL_ERROR "Please point to the correct location of the ZeroMQ library and include files.")
endif()

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# Maybe change the directory where the protobuf messages generated binaries are dropped and change this to that
include_directories(${CMAKE_CURRENT_BINARY_DIR} include ${cppzmq_INCLUDE_DIR} ${ZeroMQ_INCLUDE_DIR} ${Protobuf_INCLUDE_DIR})

file(GLOB_RECURSE Headers "*.h" "*.hpp")

set(LIB_SRCS 
  ${Headers}
  src/Server.cpp
  src/Publisher.cpp
  src/Proxy.cpp
  src/Client.cpp
	src/Subscriber.cpp 
	src/MSGcreator.cpp 
	src/MSGreader.cpp)

add_library(${PROJECT_NAME} SHARED ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME} simple_msgs ${cppzmq_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PRIVATE SIMPLE_LIBRARY_EXPORT)

## TESTS

add_subdirectory(tests)

