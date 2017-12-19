cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 

include_directories(include)
add_executable(pub_sub_test single_pub_sub.cpp)
add_executable(wrapper_point_test wrapper_point_test.cpp)
add_executable(wrapper_rotation_matrix_test wrapper_rotation_matrix_test.cpp)
add_executable(wrapper_pose_test wrapper_pose_test.cpp)
add_executable(wrapper_quaternion_test wrapper_quaternion_test.cpp)
target_link_libraries(pub_sub_test simple)
target_link_libraries(wrapper_point_test simple)
target_link_libraries(wrapper_rotation_matrix_test simple)
target_link_libraries(wrapper_pose_test simple)
target_link_libraries(wrapper_quaternion_test simple)

 
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
		)
endif()

install(TARGETS pub_sub_test wrapper_point_test wrapper_pose_test wrapper_rotation_matrix_test wrapper_quaternion_test DESTINATION bin)
