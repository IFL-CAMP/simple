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

option(SIMPLE_BUILD_SHARED "Build the shared version of the libray" TRUE)
option(SIMPLE_BUILD_STATIC "Build the static version of the libray" TRUE)
option(SIMPLE_BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(SIMPLE_BUILD_TESTS "Build SIMPLE Tests" FALSE)
option(SIMPLE_BUILD_DOC "Build SIMPLE Documentation" FALSE)
option(SIMPLE_ENABLE_CPACK "Enable packaging via CPack" FALSE)

# Examples.
if(${SIMPLE_BUILD_EXAMPLES})
  if(${SIMPLE_BUILD_STATIC})
    add_subdirectory(examples)
  else()
    message(WARNING "Not building the examples, since SIMPLE_BUILD_STATIC is not enabled.")
  endif()
endif()

# Tests.
if(${SIMPLE_BUILD_TESTS})
  if(${SIMPLE_BUILD_STATIC})
    include(CTest)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Coverage") # Coverage flags for GCC
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
if(${SIMPLE_BUILD_DOC})
  add_subdirectory(doc)
endif()

###### BUILD SIMPLE MSGS
message("CMAKE_PREFIX_PATH set to ${CMAKE_PREFIX_PATH}")
add_subdirectory(msgs)

###### FIND LIBRARIES

find_package(Threads REQUIRED)
find_package(cppzmq REQUIRED)
find_package(Protobuf REQUIRED)

###### TARGETS

# Collect all the headers.
file(GLOB SIMPLE_HEADERS "include/simple/*.hpp")

if(${SIMPLE_BUILD_SHARED})
# SHARED library.
add_library(${PROJECT_NAME} SHARED
  ${SIMPLE_HEADERS}
  src/context_manager.cpp
  src/generic_socket.cpp
  ${MSGS_HEADERS} ${MSGS_SOURCES}
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>          # For simple_export.h
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # For simple headers
  $<INSTALL_INTERFACE:include>
  PRIVATE
  $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
  $<BUILD_INTERFACE:${cppzmq_INCLUDE_DIR}>
  )

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  simple_msgs
  PRIVATE
  cppzmq-static
  ${coverage_lib}
  )

# Activate all warnings and treat them as errors.
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /w44640 /WX /permissive- /wd4127>)

# Generate the export header for the shared library.
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})
endif()

if(${SIMPLE_BUILD_STATIC})
  # Static library.
  add_library(${PROJECT_NAME}-static STATIC
    ${SIMPLE_HEADERS}
    src/context_manager.cpp
    src/generic_socket.cpp
    )

  # Required for the generated export header.
  target_compile_definitions(${PROJECT_NAME}-static PUBLIC SIMPLE_STATIC_DEFINE)

  target_include_directories(${PROJECT_NAME}-static
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>          # For simple_export.h
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # For simple headers
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${ZeroMQ_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${cppzmq_INCLUDE_DIR}>
    )

  target_link_libraries(${PROJECT_NAME}-static
    PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
    simple_msgs
    ${ZeroMQ_LIBRARY}
    PRIVATE
    $<BUILD_INTERFACE:cppzmq-static>
    ${coverage_lib}
    )

  # Activate all warnings and treat them as errors.
  target_compile_options(${PROJECT_NAME}-static PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4 /w44640 /WX /permissive- /wd4127>)
endif()

###### INSTALLATION

# Install SIMPLE in the desired folder.
if(${SIMPLE_BUILD_SHARED})
set(INSTALL_TARGETS ${PROJECT_NAME})
endif()

if(${SIMPLE_BUILD_STATIC})
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
if(${SIMPLE_ENABLE_CPACK})
  if(${SIMPLE_BUILD_STATIC})
    message(WARNING "SIMPLE packaging it not possible while SIMPLE_BUILD_STATIC is enabled.")
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
endif()
