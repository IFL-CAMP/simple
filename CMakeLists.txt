cmake_minimum_required(VERSION 3.3)
project(simple)

# Set the project version.
set(PROJECT_VER_MAJOR 1)
set(PROJECT_VER_MINOR 1)
set(PROJECT_VER_PATCH 0)
set(simple_VERSION "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allows all symbols generated from the dynamic library to be exported.
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

# Add a suffix to the debug artifacts.
set(CMAKE_DEBUG_POSTFIX "-d")

# Provides generic install paths
include(GNUInstallDirs)

# Output all binaries and libs into a common folder across all libraries.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###### UTILITIES

option(SIMPLE_USE_IWYU "FALSE")
option(SIMPLE_USE_LWYU "FALSE")
mark_as_advanced(SIMPLE_USE_IWYU SIMPLE_USE_LWYU)

if(${SIMPLE_USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find include-what-you-use")
  else()
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  endif()
endif()

if(${SIMPLE_USE_LWYU})
  set(CMAKE_LINK_WHAT_YOU_USE ON)
endif()

###### OPTIONAL BUILDS

# The shared build is not available on Windows, Protobuf and dynamic linking on Windows are not good friends. 
if (NOT WIN32)
  option(SIMPLE_BUILD_SHARED "Build the shared version of the libray" FALSE)
else()
  set(SIMPLE_BUILD_SHARED FALSE)
endif()

option(SIMPLE_BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(SIMPLE_BUILD_TESTS "Build SIMPLE Tests" FALSE)
option(SIMPLE_BUILD_DOC "Build SIMPLE Documentation" FALSE)
option(SIMPLE_ENABLE_CPACK "Enable packaging via CPack" FALSE)

# Set the name of the library target and its type from the activated options.
if (${SIMPLE_BUILD_SHARED})
  set(LIB_TARGET_NAME "simple")
  set(LIB_TYPE "SHARED")
else()
  set(LIB_TARGET_NAME "simple-static")
  set(LIB_TYPE "STATIC")
endif()

# Examples.
if(${SIMPLE_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

# Tests.
if(${SIMPLE_BUILD_TESTS})
  include(CTest)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Coverage") # Coverage flags for GCC
    set(GCC_COVERAGE_COMPILE_FLAGS "-O0 --coverage")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(coverage_lib gcov)
  endif()
  enable_testing()
  add_subdirectory(tests)
endif()

# Documentation.
if(${SIMPLE_BUILD_DOC})
  add_subdirectory(doc)
endif()

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(cppzmq REQUIRED)
find_package(Protobuf REQUIRED)

###### TARGETS

# Ignore warnings triggered by the proto messages.
if(WIN32)
  set_source_files_properties(${MSGS_HEADERS} ${MSGS_SOURCES} ${Protobuf_INCLUDE_DIRS}/* PROPERTIES COMPILE_FLAGS /W0)
endif()

add_library(${LIB_TARGET_NAME} ${LIB_TYPE}
  include/simple/client.hpp
  include/simple/context_manager.hpp
  include/simple/generic_socket.hpp
  include/simple/publisher.hpp
  include/simple/server.hpp
  include/simple/subscriber.hpp
  src/context_manager.cpp
  src/generic_socket.cpp
  ${MSGS_HEADERS} ${MSGS_SOURCES}
  )

target_include_directories(${LIB_TARGET_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>          # For simple_export.h
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/msgs>          # For simple_export.h
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # For simple headers
  $<INSTALL_INTERFACE:include>
  PRIVATE
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<BUILD_INTERFACE:${cppzmq_INCLUDE_DIR}>
  )

# Add the protobuf headers as system ones, so that compiler warnings are ignored for them.
target_include_directories(${LIB_TARGET_NAME} SYSTEM PUBLIC
  $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>
  )

target_link_libraries(${LIB_TARGET_NAME}
  PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  PRIVATE
  ${Protobuf_LIBRARIES}
  $<BUILD_INTERFACE:cppzmq-static>
  ${coverage_lib}
  )

include(GenerateExportHeader)
generate_export_header(${LIB_TARGET_NAME} BASE_NAME ${PROJECT_NAME})

# Activate all warnings and treat them as errors.
target_compile_options(${LIB_TARGET_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /w44640 /WX /permissive- /wd4127>)

# Specific settings for static builds.
if (NOT ${SIMPLE_BUILD_SHARED})
  target_compile_definitions(${LIB_TARGET_NAME} PUBLIC SIMPLE_STATIC_DEFINE)
  # simple-static needs to link directly to ZeroMQ.
  target_link_libraries(${LIB_TARGET_NAME} PUBLIC ${ZeroMQ_STATIC_LIBRARY})
  if(WIN32)
    # And on Windows this.
    target_link_libraries(${LIB_TARGET_NAME} PRIVATE ws2_32 Iphlpapi)
  endif()
endif()

###### INSTALLATION

# Install SIMPLE.
install(TARGETS ${LIB_TARGET_NAME}
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
if(${SIMPLE_ENABLE_CPACK} AND NOT ${SIMPLE_BUILD_SHARED})
  message(WARNING "SIMPLE packaging is available only when a shared library is built.")
else()
  if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/IFL-CAMP/simple")
    set(CPACK_PACKAGE_NAME "simple")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Smart Intuitive Messaging Platform with Less Effort. A Cross-Platform C++ Library to Exchange Data Across Network.")
    set(CPACK_PACKAGE_VENDOR "Chair for Computer Aided Medical Procedures - TUM")
    set(CPACK_PACKAGE_CONTACT "salvo.virga@tum.de")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VER_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VER_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VER_PATCH}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY "off")
    set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")
    string(REPLACE "." ";" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    list(GET COMPILER_VERSION 0 MAJOR_COMPILER_VERSION)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}")

    if(UNIX)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}${MAJOR_COMPILER_VERSION}")
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
    if(MSVC)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}${MSVC_VERSION}")
    endif()
      set(CPACK_GENERATOR "NSIS")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "simple")
    endif()

    # Do this at last
    include(CPack)

  endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
endif()
