cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 
 
# Allows all symbols generated from the dynamic library to be exported 
if (WIN32) 
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) 
endif(WIN32) 
 
add_executable(${PROJECT_NAME} PUB_SUB_1.cpp) 
 
target_link_libraries(${PROJECT_NAME} simple) 
 
target_compile_definitions(${PROJECT_NAME} PRIVATE SIMPLE_LIBRARY_EXPORT) # TODO is this necessary? 

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
			SHOW_PROGRESS
		)
	endif()

	include_directories(include)
 
install(TARGETS ${PROJECT_NAME} DESTINATION bin)