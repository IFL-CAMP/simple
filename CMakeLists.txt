cmake_minimum_required(VERSION 3.5)
project(simple)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)

# TODO can this be omitted if we don't use protobuf anymore?
if (MSVC)
  list(APPEND CMAKE_CXX_FLAGS_RELEASE " /MD")
  list(APPEND CMAKE_CXX_FLAGS_DEBUG " /MDd")
endif(MSVC)

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

###### OPTIONS

option(BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(BUILD_TEST "Build SIMPLE Test" FALSE)

###### BUILD SIMPLE MSGS

add_subdirectory(msgs)

###### FIND LIBRARIES

#Find Threads
find_package(Threads)

# Find ZMQ
if (WIN32)
  set(ZeroMQ_INCLUDE_DIR "" CACHE FILEPATH "Include directory for ZeroMQ")
  set(ZeroMQ_LIBRARY "" CACHE FILEPATH "The ZeroMQ library file")
  if (ZeroMQ_INCLUDE_DIR AND ZeroMQ_LIBRARY)
	set(ZeroMQ_FOUND TRUE)
  endif()
else()
  find_package(ZeroMQ REQUIRED)
endif(WIN32)

if (NOT ZeroMQ_FOUND)
  message(FATAL_ERROR "Please point to the correct location of the ZeroMQ library and its include files.")
endif()

###### FILES AND FOLDERS NEEDED FOR COMPILATION

include_directories(include msgs/include msgs/generated_include ${ZeroMQ_INCLUDE_DIR} ${FLATBUFFERS_INCLUDE_DIR})

file(GLOB SIMPLE_HEADERS "include/simple/*.h" "include/simple/*.hpp")

###### TARGETS

add_library(zmq STATIC IMPORTED)
set_target_properties(zmq PROPERTIES
  IMPORTED_LOCATION ${ZeroMQ_LIBRARY}
  INCLUDE_DIRECTORIES ${ZeroMQ_INCLUDE_DIR}
  )

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	INTERFACE_LINK_LIBRARIES zmq
)

target_link_libraries(${PROJECT_NAME} INTERFACE zmq ${CMAKE_THREAD_LIBS_INIT} simple_msgs)
target_compile_definitions(${PROJECT_NAME} INTERFACE SIMPLE_LIBRARY_EXPORT)


###### INSTALLATION

# Install SIMPLE in the desired folder
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(NOT ${ZeroMQ_INCLUDE_DIR} STREQUAL ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
   install(FILES "${ZeroMQ_INCLUDE_DIR}/zmq.h" "${ZeroMQ_INCLUDE_DIR}/zmq_utils.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(WIN32)
get_filename_component(ZMQ_LIBRARY_NAME_WO_EXT ${ZeroMQ_LIBRARY} NAME_WE)
get_filename_component(ZMQ_LIBRARY_PATH ${ZeroMQ_LIBRARY} DIRECTORY)
get_filename_component(ZMQ_PATH ${ZMQ_LIBRARY_PATH} DIRECTORY)
find_file(ZMQ_DYNAMIC_LIBRARY "${ZMQ_LIBRARY_NAME_WO_EXT}.dll"
			HINTS "${ZMQ_PATH}/bin" "${ZMQ_PATH}/lib")
  install(FILES ${ZMQ_DYNAMIC_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
  install(FILES ${ZeroMQ_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(WIN32)

include(simpleInstall.cmake.in)

###### EXAMPLES

if (${BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

###### TESTS

if (${BUILD_TESTS})
  add_subdirectory(tests)
endif()
