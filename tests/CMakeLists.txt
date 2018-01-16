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

add_executable(test_single_pub_sub test_single_pub_sub.cpp)
target_link_libraries(test_single_pub_sub simple)
add_test(NAME simple_tests.single_pub_sub COMMAND test_single_pub_sub)

add_executable(test_multi_sub test_single_pub_multi_sub.cpp)
target_link_libraries(test_multi_sub simple)
add_test(NAME simple_tests.multi_pub_sub COMMAND test_multi_sub)

add_executable(test_long_life_pub test_single_pub_long_life.cpp)
target_link_libraries(test_long_life_pub simple)
add_test(NAME simple_tests.long_life_pub COMMAND test_long_life_pub)

add_executable(test_on_off_pub test_on_off_pub.cpp)
target_link_libraries(test_on_off_pub simple)
add_test(NAME simple_tests.on_off_pub COMMAND test_on_off_pub)

add_executable(test_late_sub test_latecomer_sub.cpp)
target_link_libraries(test_late_sub simple)
add_test(NAME simple_tests.late_sub COMMAND test_late_sub)

add_executable(test_wrong_pub_sub test_wrong_pub_sub.cpp)
target_link_libraries(test_wrong_pub_sub simple)
add_test(NAME simple_tests.wrong_pub_sub COMMAND test_wrong_pub_sub) 

install(
TARGETS
   test_single_pub_sub 
   test_wrong_pub_sub 
   test_long_life_pub 
   test_on_off_pub 
   test_late_sub 
   test_multi_sub
DESTINATION bin
)
