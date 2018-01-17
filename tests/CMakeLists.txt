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

#PUB-SUB TESTS

add_executable(test_pub_sub test_pub_sub.cpp)
target_link_libraries(test_pub_sub simple-static)
add_test(NAME simple_tests.pub_sub COMMAND test_single_pub_sub)

add_executable(test_wrong_pub_sub test_wrong_pub_sub.cpp)
target_link_libraries(test_wrong_pub_sub simple-static)
add_test(NAME simple_tests.wrong_pub_sub COMMAND test_wrong_pub_sub) 

install(
TARGETS
   test_pub_sub 
   test_wrong_pub_sub 
DESTINATION bin
)
