cmake_minimum_required(VERSION 3.5)
project(simple)

# Set the project version
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER_MINOR 1)
set(PROJECT_VER_MAJOR 1)
set(simple_VERSION "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")

include(GNUInstallDirs)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

set(CMAKE_DEBUG_POSTFIX "-d")

###### OPTIONS

option(BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(BUILD_TESTS "Build SIMPLE Test" FALSE)

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(ZeroMQ REQUIRED)

###### TARGETS

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} INTERFACE
  libzmq ${CMAKE_THREAD_LIBS_INIT} simple_msgs)

add_library(${PROJECT_NAME}-static INTERFACE)

target_include_directories(${PROJECT_NAME}-static INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME}-static INTERFACE
  libzmq-static ${CMAKE_THREAD_LIBS_INIT} simple_msgs)

###### INSTALLATION

# Install SIMPLE in the desired folder
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static
  EXPORT "simpleTargets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install SIMPLE includes
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_SOURCE_DIR}/simpleConfigVersion.cmake"
  VERSION ${simple_VERSION}
  COMPATIBILITY SameMajorVersion)

install (FILES "simpleConfig.cmake" "simpleConfigVersion.cmake"
  DESTINATION lib/cmake/simple)

install(EXPORT simpleTargets
  FILE simpleTargets.cmake
  NAMESPACE simple::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simple)

###### EXAMPLES

if (${BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

###### TESTS

if (${BUILD_TESTS})
  enable_testing()
  add_subdirectory(tests)
endif()
