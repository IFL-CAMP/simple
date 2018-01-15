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
add_executable(test_double test_double.cpp)
target_link_libraries(test_double simple)
add_test(NAME simple_tests.double COMMAND test_double)

add_executable(test_float test_float.cpp)
target_link_libraries(test_float simple)
add_test(NAME simple_tests.float COMMAND test_float)

add_executable(test_int test_int.cpp)
target_link_libraries(test_int simple)
add_test(NAME simple_tests.int COMMAND test_int)

add_executable(test_point test_point.cpp)
target_link_libraries(test_point simple)
add_test(NAME simple_tests.point COMMAND test_point)

add_executable(test_rotation_matrix test_rotation_matrix.cpp)
target_link_libraries(test_rotation_matrix simple)
add_test(NAME simple_tests.matrix COMMAND test_rotation_matrix)

add_executable(test_pose test_pose.cpp)
target_link_libraries(test_pose simple)
add_test(NAME simple_tests.pose COMMAND test_pose)

add_executable(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion simple)
add_test(NAME simple_tests.quaternion COMMAND test_quaternion)

add_executable(test_header test_header.cpp)
target_link_libraries(test_header simple)
add_test(NAME simple_tests.header COMMAND test_header)

add_executable(test_image test_image.cpp)
target_link_libraries(test_image simple)
add_test(NAME simple_tests.image COMMAND test_image)

add_executable(test_pose_stamped test_pose_stamped.cpp)
target_link_libraries(test_pose_stamped simple)
add_test(NAME simple_tests.pose_stamped COMMAND test_pose_stamped)


install(
TARGETS
  test_double
  test_float
  test_int
  test_point
  test_pose
  test_rotation_matrix
  test_quaternion
  test_header
  test_image
  test_pose_stamped
DESTINATION 
	bin
)
