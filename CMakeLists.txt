cmake_minimum_required(VERSION 3.5)
project(simple)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

###### OPTIONS

option(BUILD_EXAMPLES "Build SIMPLE Examples" FALSE)
option(BUILD_TESTS "Build SIMPLE Test" FALSE)

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
	get_filename_component(ZMQ_LIBRARY_NAME_WE ${ZeroMQ_LIBRARY} NAME_WE)
	get_filename_component(ZMQ_LIBRARY_PATH ${ZeroMQ_LIBRARY} DIRECTORY)
	get_filename_component(ZMQ_PATH ${ZMQ_LIBRARY_PATH} DIRECTORY)
	set(ZMQ_INSTALL_NAME ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${ZMQ_LIBRARY_NAME_WE}.lib)
else()
	set(ZMQ_INSTALL_NAME ${ZeroMQ_LIBRARY})
endif()

if (WIN32)
	get_target_property(SIMPLE_MSGS_NAME simple_msgs OUTPUT_NAME)
	set(SIMPLE_MSGS_INSTALL_NAME ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${SIMPLE_MSGS_NAME}.lib)
else()
	set(SIMPLE_MSGS_INSTALL_NAME ${SIMPLE_MSGS_NAME})

endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE 
$<BUILD_INTERFACE:${ZeroMQ_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} simple_msgs>
$<INSTALL_INTERFACE:${ZMQ_INSTALL_NAME} ${SIMPLE_MSGS_INSTALL_NAME}>
)

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
install(FILES 	"${ZeroMQ_INCLUDE_DIR}/zmq.h" 
				"${ZeroMQ_INCLUDE_DIR}/zmq_utils.h" 
				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# Install Flatbuffers includes
install(
FILES 	${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/flatbuffers.h 
		${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/base.h 
		${FLATBUFFERS_INCLUDE_DIR}/flatbuffers/stl_emulation.h 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flatbuffers/)

# Install ZeroMQ lib
if(WIN32)
	find_file(ZMQ_DYNAMIC_LIBRARY "${ZMQ_LIBRARY_NAME_WE}.dll"
			HINTS "${ZMQ_PATH}/bin" "${ZMQ_PATH}/lib")
	find_file(ZMQ_LIBRARY "${ZMQ_LIBRARY_NAME_WE}.lib"
			HINTS "${ZMQ_PATH}/bin" "${ZMQ_PATH}/lib")
	install(FILES ${ZMQ_DYNAMIC_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR})  
	install(FILES ${ZMQ_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})  
else()
  install(FILES ${ZeroMQ_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(WIN32)

mark_as_advanced(ZMQ_LIBRARY ZMQ_DYNAMIC_LIBRARY)

include(simpleInstall.cmake.in)

###### EXAMPLES

if (${BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

###### TESTS

if (${BUILD_TESTS})
  add_subdirectory(tests)
endif()
