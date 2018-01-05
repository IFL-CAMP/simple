cmake_minimum_required(VERSION 3.5) 
project(simple-tests) 

include_directories(include)

#TESTS FOR WRAPPERS

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

install(TARGETS wrapper_point_test wrapper_pose_test wrapper_rotation_matrix_test wrapper_quaternion_test DESTINATION bin)
