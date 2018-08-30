cmake_minimum_required(VERSION 3.3)
project(simple)

# Set the project version.
set(PROJECT_VER_PATCH 0)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_MAJOR 1)
set(simple_VERSION "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allows all symbols generated from the dynamic library to be exported.
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

set(CMAKE_DEBUG_POSTFIX "-d")

# Output all binaries and libs into a common folder across all libraries.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the compiler options to the given target according to the current architecture.
function(simple_compile_options target_name)
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /W4 /w44640 /WX)
    if (MSVC_VERSION GREATER 1900)
      target_compile_options(${target_name} PRIVATE /permissive-)
    endif()
  else()
    target_compile_options(${target_name} PRIVATE -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
  endif()
endfunction(simple_compile_options)

###### OPTIONAL BUILDS

option(SIMPLE_BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(SIMPLE_BUILD_TESTS "Build SIMPLE Tests" FALSE)

# Examples.
if (${SIMPLE_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

# Tests.
if (${SIMPLE_BUILD_TESTS})
  include(CTest)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Coverage") # Coverage flags for GCC
    add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
    set(coverage_lib gcov)
  endif()
  enable_testing()
  add_subdirectory(tests)
endif()

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(ZeroMQ REQUIRED)

###### TARGETS

# Collect all the headers.
file(GLOB SIMPLE_HEADERS "include/simple/*.hpp")

# Dummy target to visualize the header files in some IDE.
add_custom_target(${PROJECT_NAME}-ide SOURCES ${SIMPLE_HEADERS})

# SHARED library.
add_library(${PROJECT_NAME} SHARED src/context_manager.cpp)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  PRIVATE
    $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    simple_msgs
    libzmq-static
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${coverage_lib}
)

# Set the compiler options for the shared lib, all warning are activated and treaded as errors.
simple_compile_options(${PROJECT_NAME})

# Generate the export header for the shared library.
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# STATIC library.
add_library(${PROJECT_NAME}-static src/context_manager.cpp)

# Required for the generated export header.
target_compile_definitions(${PROJECT_NAME}-static PUBLIC SIMPLE_STATIC_DEFINE)

target_include_directories(${PROJECT_NAME}-static PUBLIC
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  PRIVATE
    $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME}-static
  PUBLIC
    simple_msgs
    libzmq-static
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${coverage_lib}
)

# Set the compiler options for the static lib, all warning are activated and treaded as errors.
simple_compile_options(${PROJECT_NAME}-static)

###### INSTALLATION

# Install SIMPLE in the desired folder.
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static
  EXPORT "simpleTargets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install SIMPLE includes.
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
