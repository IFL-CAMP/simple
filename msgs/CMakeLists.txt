cmake_minimum_required(VERSION 3.6)
project(simple_msgs)

# Add extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../cmake/Modules)

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

# Find FlatBuffers
find_package(FlatBuffers REQUIRED)

include_directories(include generated_include ${FLATBUFFERS_INCLUDE_DIR})

file(GLOB_RECURSE SIMPLE_MSGS_HEADERS "*.h" "*.hpp")

set(SIMPLE_MSGS_SRCS
  ${SIMPLE_MSGS_HEADERS}
  src/header.cpp
  src/image.cpp
  src/pose.cpp
  src/quaternion.cpp
  src/point.cpp
  )

# Generate the Flatbuffers headers - this is defined in FindFlatBuffers.cmake
flatbuffers_generate_c_headers(simple_msgs ${PROJECT_SOURCE_DIR}/fbs ${PROJECT_SOURCE_DIR}/generated_include)

add_library(${PROJECT_NAME} SHARED ${SIMPLE_MSGS_SRCS})

target_compile_definitions(${PROJECT_NAME} PRIVATE SIMPLE_LIBRARY_EXPORT) # TODO is this necessary?

###### INSTALATION

#install the simple_msgs library in the desired folder
install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})