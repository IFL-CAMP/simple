cmake_minimum_required(VERSION 3.3)
project(simple)

# Set the project version
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_MAJOR 1)
set(simple_VERSION "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

if(MSVC)
  add_compile_options(/W4 /w44640 /WX)
  if (MSVC_VERSION GREATER 1900)
    add_compile_options(/permissive-)
  endif()
else()
  add_compile_options(-Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()

###### OPTIONAL BUILDS

option(SIMPLE_BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(SIMPLE_BUILD_TESTS "Build SIMPLE Tests" FALSE)
option(SIMPLE_BUILD_DOC "Build SIMPLE Documentation" FALSE)

# Examples
if (${SIMPLE_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

# Tests
if (${SIMPLE_BUILD_TESTS})
  include(CTest)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU") # Coverage flags for GCC
    add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
    set(coverage_lib gcov)
  endif()
  enable_testing()
  add_subdirectory(tests)
endif()

# Documentation
if (${SIMPLE_BUILD_DOC})
  add_subdirectory(doc)
endif()

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(ZeroMQ REQUIRED)

###### TARGETS

file(GLOB SIMPLE_HEADERS "include/simple/*.hpp")
add_custom_target(${PROJECT_NAME}-ide SOURCES ${SIMPLE_HEADERS})

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} INTERFACE
  libzmq ${CMAKE_THREAD_LIBS_INIT} simple_msgs ${coverage_lib})

add_library(${PROJECT_NAME}-static INTERFACE)

target_include_directories(${PROJECT_NAME}-static INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME}-static INTERFACE
  libzmq-static ${CMAKE_THREAD_LIBS_INIT} simple_msgs ${coverage_lib})

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
