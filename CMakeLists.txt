cmake_minimum_required(VERSION 3.3)
project(simple)

# Set the project version.
set(PROJECT_VER_MAJOR 1)
set(PROJECT_VER_MINOR 0)
set(PROJECT_VER_PATCH 0)
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

###### UTILITIES

option(SIMPLE_USE_IWYU "FALSE")
option(SIMPLE_USE_LWYU "FALSE")
mark_as_advanced(SIMPLE_USE_IWYU SIMPLE_USE_LWYU)

if (${SIMPLE_USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find include-what-you-use")
  else()
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  endif()
endif()

if (${SIMPLE_USE_LWYU})
  set(CMAKE_LINK_WHAT_YOU_USE ON)
endif()

###### OPTIONAL BUILDS

option(SIMPLE_BUILD_STATIC "Build the static version of the libray" TRUE)
option(SIMPLE_BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(SIMPLE_BUILD_TESTS "Build SIMPLE Tests" FALSE)
option(SIMPLE_BUILD_DOC "Build SIMPLE Documentation" FALSE)
option(SIMPLE_CPACK "Enable packaging via CPack" FALSE)

# Examples.
if (${SIMPLE_BUILD_EXAMPLES})
  if (${SIMPLE_BUILD_STATIC})
    add_subdirectory(examples)
  else()
    message(WARNING "Not building the examples, since SIMPLE_BUILD_STATIC is not enabled.")
  endif()
endif()

# Tests.
if (${SIMPLE_BUILD_TESTS})
  if (${SIMPLE_BUILD_STATIC})
    include(CTest)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Coverage") # Coverage flags for GCC
      add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
      set(coverage_lib gcov)
    endif()
    enable_testing()
    add_subdirectory(tests)
  else()
    message(WARNING "Not building the tests, since SIMPLE_BUILD_STATIC is not enabled.")
  endif()
endif()

# Documentation.
if (${SIMPLE_BUILD_DOC})
  add_subdirectory(doc)
endif()

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(cppzmq REQUIRED)

###### TARGETS

# Collect all the headers.
file(GLOB SIMPLE_HEADERS "include/simple/*.hpp")

# Dummy target to visualize the header files in some IDE.
add_custom_target(${PROJECT_NAME}-ide SOURCES ${SIMPLE_HEADERS})

# SHARED library.
add_library(${PROJECT_NAME} SHARED
  src/context_manager.cpp
  src/generic_socket.cpp
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
  PRIVATE
    $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${cppzmq_INCLUDE_DIR}>
    )

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    simple_msgs
    ${CMAKE_THREAD_LIBS_INIT}
  PRIVATE
    cppzmq-static
    ${coverage_lib}
    )

# Set the compiler options for the shared lib, all warning are activated and treated as errors.
simple_compile_options(${PROJECT_NAME})

# Generate the export header for the shared library.
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# STATIC library.

if (${SIMPLE_BUILD_STATIC})
  add_library(${PROJECT_NAME}-static STATIC
    src/context_manager.cpp
    src/generic_socket.cpp
    )

  # Required for the generated export header.
  target_compile_definitions(${PROJECT_NAME}-static PUBLIC SIMPLE_STATIC_DEFINE)

  target_include_directories(${PROJECT_NAME}-static
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/msgs/include>
    PRIVATE
      $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
      $<BUILD_INTERFACE:${cppzmq_INCLUDE_DIR}>
      )

  target_link_libraries(${PROJECT_NAME}-static
    PUBLIC
      simple_msgs
      ${CMAKE_THREAD_LIBS_INIT}
    PRIVATE
      ${coverage_lib}
      $<BUILD_INTERFACE:cppzmq-static>
      )

  # Set the compiler options for the static lib, all warning are activated and treaded as errors.
  simple_compile_options(${PROJECT_NAME}-static)
endif()

###### INSTALLATION

# Install SIMPLE in the desired folder.

set(INSTALL_TARGETS ${PROJECT_NAME})
if()
  list(APPEND INSTALL_TARGETS ${PROJECT_NAME}-static)
endif()

install(TARGETS ${INSTALL_TARGETS}
  EXPORT "simpleTargets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

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

###### CPACK

if(${SIMPLE_CPACK})
  if(${SIMPLE_BUILD_STATIC})
    message(WARNING "SIMPLE packaging it not possible while SIMPLE_BUILD_STATIC is enabled.")
  else()
    if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

      set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/IFL-CAMP/simple")
      set(CPACK_PACKAGE_NAME "simple")
      set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Smart Intuitive Messaging Platform with Less Effort. A Cross-Platform C++ Library to Exchange Data Across Network.")
      set(CPACK_PACKAGE_VENDOR "Chair for Computer Aided Medical Procedures - TUM")
      set(CPACK_PACKAGE_CONTACT "Salvo Virga")
      set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VER_MAJOR}")
      set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VER_MINOR}")
      set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VER_PATCH}")
      set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
      set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
      set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY "off")
      set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

      if(UNIX)
        if(APPLE)
          # MacOS
          set(CPACK_GENERATOR "TGZ")
        else()
          # Linux
          set(CPACK_GENERATOR "DEB")
          set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
          set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
          set(CPACK_DEBIAN_PACKAGE_SECTION "science")
          if(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
              execute_process(COMMAND dpkg --print-architecture
                  OUTPUT_VARIABLE arch OUTPUT_STRIP_TRAILING_WHITESPACE)
              set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${arch}")
          endif()
        endif()
      elseif(WIN32)
        set(CPACK_GENERATOR "NSIS")
      endif()

      # Do this last
      include(CPack)

    endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  endif()
endif()
