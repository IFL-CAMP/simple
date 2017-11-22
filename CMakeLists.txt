cmake_minimum_required(VERSION 3.6)
project(simple)

###### GLOBAL PARAMETERS AND SETTINGS

# Here all the compiler options for all projects created are set
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add extra CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules)

# TODO can this be omitted if we don't use protobuf anymore?
if (WIN32)
  list(APPEND CMAKE_CXX_FLAGS_RELEASE " /MT")
  list(APPEND CMAKE_CXX_FLAGS_DEBUG " /MTd")
endif(WIN32)

# Allows all symbols generated from the dynamic library to be exported
if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif(WIN32)

###### FIND LIBRARIES

# Find FlatBuffers
find_package(FlatBuffers REQUIRED)

# Find ZMQ
if (WIN32)
  set(ZeroMQ_INCLUDE_DIR "" CACHE FILEPATH "Include directory for ZeroMQ")
  set(cppzmq_LIBRARY "" CACHE FILEPATH "The ZeroMQ library file")
  set(cppzmq_INCLUDE_DIR "" CACHE FILEPATH "Include directory for cppzmq")
else()
  find_package(cppzmq REQUIRED)
endif(WIN32)

if ((NOT ((ZeroMQ_INCLUDE_DIR) AND (cppzmq_LIBRARY) AND (cppzmq_INCLUDE_DIR))))
  message(FATAL_ERROR "Please point to the correct location of the ZeroMQ library and its include files.")
endif()

###### FILES AND FOLDERS NEEDED FOR COMPILATION

include_directories(include simple_msgs/include ${cppzmq_INCLUDE_DIR} ${ZeroMQ_INCLUDE_DIR} ${FLATBUFFERS_INCLUDE_DIR})

file(GLOB_RECURSE Headers "*.h" "*.hpp")

set(LIB_SRCS 
  ${Headers}
  src/Server.cpp
  src/Publisher.cpp
  src/Proxy.cpp
  src/Client.cpp
	src/Subscriber.cpp
	src/MSGcreator.cpp
	src/MSGreader.cpp)

###### TARGETS

# Generate the Flatbuffers headers - this is defined in FindFlatBuffers.cmake
flatbuffers_generate_c_headers(simple_msgs ${PROJECT_SOURCE_DIR}/msgs/fbs ${PROJECT_SOURCE_DIR}/msgs/include) #TODO check that the compilation of the the flatbuffer header in Windows happens correctly

add_library(${PROJECT_NAME} SHARED ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME} ${cppzmq_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PRIVATE SIMPLE_LIBRARY_EXPORT) # TODO is this necessary?


###### INSTALLATION

#install the simple library in the desired folder
install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}) # TODO check if ok

#install the ZMQ library in the desired folder, for Windows
if(WIN32)
  get_filename_component(ZMQ_LIBRARY_NAME_WO_EXT ${cppzmq_LIBRARY} NAME_WE)
  get_filename_component(ZMQ_LIBRARY_PATH ${cppzmq_LIBRARY} DIRECTORY)
  set(ZMQ_DYNAMIC_LIBRARY "${ZMQ_LIBRARY_PATH}/../bin/${ZMQ_LIBRARY_NAME_WO_EXT}.dll")
  install(FILES ${ZMQ_DYNAMIC_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR}) # TODO: check this variable on windows
endif(WIN32)

###### TESTS

add_subdirectory(tests)

