cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
		)
endif()

include_directories(include)

#REQ-REP TESTS

add_executable(test_single_req_rep test_single_req_rep.cpp)
target_link_libraries(test_single_req_rep simple-static)
add_test(NAME simple_tests.single_req_rep COMMAND test_single_req_rep)

add_executable(test_multi_req_rep test_multi_req_rep.cpp)
target_link_libraries(test_multi_req_rep simple-static)
add_test(NAME simple_tests.multi_req_rep COMMAND test_multi_req_rep)

install(
TARGETS 
  test_single_req_rep
  test_multi_req_rep 
DESTINATION bin
)
