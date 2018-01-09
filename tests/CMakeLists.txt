cmake_minimum_required(VERSION 3.5) 
project(simple_tests)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/include/catch.hpp)
		message(STATUS "Downloading catch.hpp. This may take a while.")
		file(DOWNLOAD
			"https://github.com/philsquared/Catch/releases/download/v2.0.1/catch.hpp"
			"${PROJECT_SOURCE_DIR}/include/catch.hpp"
		)
endif()

include_directories(include)

#TESTS FOR WRAPPERS
add_executable(double_test double_test.cpp)
target_link_libraries(double_test simple-static)
add_test(NAME simple_tests.double COMMAND double_test)

add_executable(float_test float_test.cpp)
target_link_libraries(float_test simple-static)
add_test(NAME simple_tests.float COMMAND float_test)

add_executable(int_test int_test.cpp)
target_link_libraries(int_test simple-static)
add_test(NAME simple_tests.int COMMAND int_test)

add_executable(point_test point_test.cpp)
target_link_libraries(point_test simple-static)
add_test(NAME simple_tests.point COMMAND point_test)

add_executable(rotation_matrix_test rotation_matrix_test.cpp)
target_link_libraries(rotation_matrix_test simple-static)
add_test(NAME simple_tests.matrix COMMAND rotation_matrix_test)

add_executable(pose_test pose_test.cpp)
target_link_libraries(pose_test simple-static)
add_test(NAME simple_tests.pose COMMAND pose_test)

add_executable(quaternion_test quaternion_test.cpp)
target_link_libraries(quaternion_test simple-static)
add_test(NAME simple_tests.quaternion COMMAND quaternion_test)

add_executable(header_test header_test.cpp)
target_link_libraries(header_test simple-static)
add_test(NAME simple_tests.header COMMAND header_test)

add_executable(image_test image_test.cpp)
target_link_libraries(image_test simple-static)
add_test(NAME simple_tests.image COMMAND image_test)

install(
TARGETS
  double_test
  float_test
  int_test
  point_test
  pose_test
  rotation_matrix_test
  quaternion_test
  header_test
  image_test
DESTINATION 
	bin
)
