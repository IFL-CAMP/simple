cmake_minimum_required(VERSION 3.5)
project(simple_msgs)

# Add extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../cmake/Modules)

# Find FlatBuffers
find_package(FlatBuffers REQUIRED)

include_directories(include generated_include ${FLATBUFFERS_INCLUDE_DIR})

file(GLOB_RECURSE SIMPLE_MSGS_HEADERS "*.h" "*.hpp")

set(SIMPLE_MSGS_SRCS
  ${SIMPLE_MSGS_HEADERS}
  src/bool.cpp
  src/string.cpp
  src/header.cpp
  src/image.cpp
  src/point.cpp
  src/point_stamped.cpp
  src/quaternion.cpp
  src/quaternion_stamped.cpp
  src/pose.cpp
  src/pose_stamped.cpp
  src/rotation_matrix.cpp
  src/rotation_matrix_stamped.cpp
  )

# Generate the Flatbuffers headers - this is defined in FindFlatBuffers.cmake
flatbuffers_generate_c_headers(simple_msgs ${PROJECT_SOURCE_DIR}/fbs ${PROJECT_SOURCE_DIR}/include/simple_msgs/generated)

add_library(${PROJECT_NAME} SHARED ${SIMPLE_MSGS_SRCS})
if (WIN32)
set_target_properties (${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "DLL_EXPORT"
    OUTPUT_NAME "libsimple_msgs")
else()
set_target_properties (${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "simple_msgs")
endif()

###### INSTALLATION

#install the simple_msgs library in the desired folder
install(TARGETS ${PROJECT_NAME} 
    EXPORT "simpleTargets"
	  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
	
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
