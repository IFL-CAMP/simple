cmake_minimum_required(VERSION 3.6)
project(simple_msgs)

# Add extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../cmake/Modules)

# Find FlatBuffers
find_package(FlatBuffers REQUIRED)

include_directories(include generated_include ${FLATBUFFERS_INCLUDE_DIR})

file(GLOB_RECURSE SIMPLE_MSGS_HEADERS "*.h" "*.hpp")

set(SIMPLE_MSGS_SRCS
  ${SIMPLE_MSGS_HEADERS}
  src/header.cpp
  )

# Generate the Flatbuffers headers - this is defined in FindFlatBuffers.cmake
flatbuffers_generate_c_headers(simple_msgs ${PROJECT_SOURCE_DIR}/fbs ${PROJECT_SOURCE_DIR}/generated_include)

add_library(${PROJECT_NAME} SHARED ${SIMPLE_MSGS_SRCS})

