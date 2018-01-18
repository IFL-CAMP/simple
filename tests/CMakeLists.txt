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
target_link_libraries(test_double simple-static)
add_test(NAME simple_tests.double COMMAND $<TARGET_FILE:test_double>)

add_executable(test_float test_float.cpp)
target_link_libraries(test_float simple-static)
add_test(NAME simple_tests.float COMMAND $<TARGET_FILE:test_float>)

add_executable(test_int test_int.cpp)
target_link_libraries(test_int simple-static)
add_test(NAME simple_tests.int COMMAND $<TARGET_FILE:test_int>)

add_executable(test_point test_point.cpp)
target_link_libraries(test_point simple-static)
add_test(NAME simple_tests.point COMMAND $<TARGET_FILE:test_point>)

add_executable(test_rotation_matrix test_rotation_matrix.cpp)
target_link_libraries(test_rotation_matrix simple-static)
add_test(NAME simple_tests.rotation_matrix COMMAND $<TARGET_FILE:test_rotation_matrix>)

add_executable(test_pose test_pose.cpp)
target_link_libraries(test_pose simple-static)
add_test(NAME simple_tests.pose COMMAND $<TARGET_FILE:test_pose>)

add_executable(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion simple-static)
add_test(NAME simple_tests.quaternion COMMAND $<TARGET_FILE:test_quaternion>)

add_executable(test_header test_header.cpp)
target_link_libraries(test_header simple-static)
add_test(NAME simple_tests.header COMMAND $<TARGET_FILE:test_header>)

add_executable(test_image test_image.cpp)
target_link_libraries(test_image simple-static)
add_test(NAME simple_tests.image COMMAND $<TARGET_FILE:test_image>)

add_executable(test_pose_stamped test_pose_stamped.cpp)
target_link_libraries(test_pose_stamped simple-static)
add_test(NAME simple_tests.pose_stamped COMMAND $<TARGET_FILE:test_pose_stamped>)

add_executable(test_point_stamped test_point_stamped.cpp)
target_link_libraries(test_point_stamped simple-static)
add_test(NAME simple_tests.point_stamped COMMAND $<TARGET_FILE:test_point_stamped>)

add_executable(test_quaternion_stamped test_quaternion_stamped.cpp)
target_link_libraries(test_quaternion_stamped simple-static)
add_test(NAME simple_tests.quaternion_stamped COMMAND $<TARGET_FILE:test_quaternion_stamped>)

add_executable(test_rotation_matrix_stamped test_rotation_matrix_stamped.cpp)
target_link_libraries(test_rotation_matrix_stamped simple-static)
add_test(NAME simple_tests.rotation_matrix_stamped COMMAND $<TARGET_FILE:test_rotation_matrix_stamped>)

# PUBLISHER / SUBSCRIBER TESTS
add_executable(test_pub_sub test_pub_sub.cpp)
target_link_libraries(test_pub_sub simple-static)
add_test(NAME simple_tests.pub_sub COMMAND $<TARGET_FILE:test_pub_sub>)

add_executable(test_wrong_pub_sub test_wrong_pub_sub.cpp)
target_link_libraries(test_wrong_pub_sub simple-static)
add_test(NAME simple_tests.wrong_pub_sub COMMAND $<TARGET_FILE:test_wrong_pub_sub>)

# CLIENT / SERVER TESTS

add_executable(test_req_rep test_single_req_rep.cpp)
target_link_libraries(test_req_rep simple-static)
add_test(NAME simple_tests.req_rep COMMAND $<TARGET_FILE:test_req_rep>)

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
  test_point_stamped
  test_quaternion_stamped
  test_rotation_matrix_stamped
  test_pub_sub 
  test_wrong_pub_sub 
  test_req_rep
DESTINATION 
	bin
)
