cmake_minimum_required(VERSION 3.5)
project(simple)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  set(ZeroMQ_INCLUDE_DIR "" CACHE PATH "Include directory for ZeroMQ")
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
add_library(zmq SHARED IMPORTED)
if (WIN32)
get_filename_component(ZMQ_LIBRARY_NAME_WO_EXT ${ZeroMQ_LIBRARY} NAME_WE)
get_filename_component(ZMQ_LIBRARY_PATH ${ZeroMQ_LIBRARY} DIRECTORY)
get_filename_component(ZMQ_PATH ${ZMQ_LIBRARY_PATH} DIRECTORY)
set(ZMQ_INSTALL_NAME ${ZMQ_LIBRARY_NAME_WO_EXT}.lib)
else()
set(ZMQ_INSTALL_NAME ${ZeroMQ_LIBRARY})
endif()

set_target_properties(zmq PROPERTIES
PUBLIC_HEADERS ${ZeroMQ_INCLUDE_DIR}
IMPORTED_LOCATION ${CMAKE_INSTALL_LIBDIR}/${ZMQ_INSTALL_NAME}
INCLUDE_DIRECTORIES ${ZeroMQ_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE ${ZeroMQ_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} simple_msgs)
target_compile_definitions(${PROJECT_NAME} INTERFACE SIMPLE_LIBRARY_EXPORT)

###### INSTALLATION

include(GNUInstallDirs)

# Install SIMPLE in the desired folder
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
# Install SIMPLE includes
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install ZeroMQ includes
if(NOT ${ZeroMQ_INCLUDE_DIR} STREQUAL ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
   install(FILES "${ZeroMQ_INCLUDE_DIR}/zmq.h" "${ZeroMQ_INCLUDE_DIR}/zmq_utils.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
# Install Flatbuffers includes
if(NOT ${FLATBUFFERS_INCLUDE_DIR} STREQUAL ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
  install(
  FILES ${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/flatbuffers.h 
		${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/base.h 
		${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/stl_emulation.h 
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flatbuffers/)
endif()

# Install ZeroMQ lib
if(WIN32)
	find_file(ZMQ_DYNAMIC_LIBRARY "${ZMQ_LIBRARY_NAME_WO_EXT}.dll"
			HINTS "${ZMQ_PATH}/bin" "${ZMQ_PATH}/lib")
	find_file(ZMQ_LIBRARY "${ZMQ_LIBRARY_NAME_WO_EXT}.lib"
			HINTS "${ZMQ_PATH}/bin" "${ZMQ_PATH}/lib")
	install(FILES ${ZMQ_DYNAMIC_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR})  
	install(FILES ${ZMQ_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})  
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
