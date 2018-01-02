cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 

include_directories(include)

#TESTS

add_executable(single_pub_sub_test single_pub_sub.cpp)
target_link_libraries(single_pub_sub_test simple)

add_executable(multi_sub_test single_pub_multi_sub.cpp)
target_link_libraries(multi_sub_test simple)

add_executable(long_life_pub_test single_pub_long_life.cpp)
target_link_libraries(long_life_pub_test simple)

add_executable(on_off_pub_test on_off_pub.cpp)
target_link_libraries(on_off_pub_test simple)

add_executable(late_sub_test latecomer_sub.cpp)
target_link_libraries(late_sub_test simple)

add_executable(wrapper_point_test wrapper_point_test.cpp)
target_link_libraries(wrapper_point_test simple)

add_executable(wrapper_rotation_matrix_test wrapper_rotation_matrix_test.cpp)
target_link_libraries(wrapper_rotation_matrix_test simple)

add_executable(wrapper_pose_test wrapper_pose_test.cpp)
target_link_libraries(wrapper_pose_test simple)

add_executable(wrapper_quaternion_test wrapper_quaternion_test.cpp)
target_link_libraries(wrapper_quaternion_test simple)

 
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
		)
endif()

install(TARGETS single_pub_sub_test long_life_pub_test on_off_pub_test late_sub_test multi_sub_test  wrapper_point_test wrapper_pose_test wrapper_rotation_matrix_test wrapper_quaternion_test DESTINATION bin)
