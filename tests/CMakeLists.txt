cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 

include_directories(include)

#REQ-REP TESTS

add_executable(single_req_rep_test single_req_rep.cpp)
target_link_libraries(single_req_rep_test simple)

add_executable(multi_req_rep_test multi_req_rep.cpp)
target_link_libraries(multi_req_rep_test simple)
 
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
		)
endif()

install(TARGETS single_req_rep_test multi_req_rep_test DESTINATION bin)
