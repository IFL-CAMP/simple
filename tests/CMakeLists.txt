cmake_minimum_required(VERSION 3.3) 
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
add_executable(test_bool test_bool.cpp)
target_link_libraries(test_bool simple-static ${coverage_lib})
add_test(NAME simple_tests.bool COMMAND $<TARGET_FILE:test_bool>)

add_executable(test_string test_string.cpp)
target_link_libraries(test_string simple-static ${coverage_lib})
add_test(NAME simple_tests.string COMMAND $<TARGET_FILE:test_string>)

add_executable(test_double test_double.cpp)
target_link_libraries(test_double simple-static ${coverage_lib})
add_test(NAME simple_tests.double COMMAND $<TARGET_FILE:test_double>)

add_executable(test_float test_float.cpp)
target_link_libraries(test_float simple-static ${coverage_lib})
add_test(NAME simple_tests.float COMMAND $<TARGET_FILE:test_float>)

add_executable(test_int test_int.cpp)
target_link_libraries(test_int simple-static ${coverage_lib})
add_test(NAME simple_tests.int COMMAND $<TARGET_FILE:test_int>)

add_executable(test_point test_point.cpp)
target_link_libraries(test_point simple-static ${coverage_lib})
add_test(NAME simple_tests.point COMMAND $<TARGET_FILE:test_point>)

add_executable(test_rotation_matrix test_rotation_matrix.cpp)
target_link_libraries(test_rotation_matrix simple-static ${coverage_lib})
add_test(NAME simple_tests.rotation_matrix COMMAND $<TARGET_FILE:test_rotation_matrix>)

add_executable(test_pose test_pose.cpp)
target_link_libraries(test_pose simple-static ${coverage_lib})
add_test(NAME simple_tests.pose COMMAND $<TARGET_FILE:test_pose>)

add_executable(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion simple-static ${coverage_lib})
add_test(NAME simple_tests.quaternion COMMAND $<TARGET_FILE:test_quaternion>)

add_executable(test_header test_header.cpp)
target_link_libraries(test_header simple-static ${coverage_lib})
add_test(NAME simple_tests.header COMMAND $<TARGET_FILE:test_header>)

add_executable(test_image test_image.cpp)
target_link_libraries(test_image simple-static ${coverage_lib})
add_test(NAME simple_tests.image COMMAND $<TARGET_FILE:test_image>)

add_executable(test_pose_stamped test_pose_stamped.cpp)
target_link_libraries(test_pose_stamped simple-static ${coverage_lib})
add_test(NAME simple_tests.pose_stamped COMMAND $<TARGET_FILE:test_pose_stamped>)

add_executable(test_point_stamped test_point_stamped.cpp)
target_link_libraries(test_point_stamped simple-static ${coverage_lib})
add_test(NAME simple_tests.point_stamped COMMAND $<TARGET_FILE:test_point_stamped>)

add_executable(test_quaternion_stamped test_quaternion_stamped.cpp)
target_link_libraries(test_quaternion_stamped simple-static ${coverage_lib})
add_test(NAME simple_tests.quaternion_stamped COMMAND $<TARGET_FILE:test_quaternion_stamped>)

add_executable(test_rotation_matrix_stamped test_rotation_matrix_stamped.cpp)
target_link_libraries(test_rotation_matrix_stamped simple-static ${coverage_lib})
add_test(NAME simple_tests.rotation_matrix_stamped COMMAND $<TARGET_FILE:test_rotation_matrix_stamped>)

# PUBLISHER / SUBSCRIBER TESTS
add_executable(test_publisher test_publisher.cpp)
target_link_libraries(test_publisher simple-static ${coverage_lib})
add_test(NAME simple_tests.publisher COMMAND $<TARGET_FILE:test_publisher>)

add_executable(test_subscriber test_subscriber.cpp)
target_link_libraries(test_subscriber simple-static ${coverage_lib})
add_test(NAME simple_tests.subscriber COMMAND $<TARGET_FILE:test_subscriber>)

add_executable(test_pub_sub test_pub_sub.cpp)
target_link_libraries(test_pub_sub simple-static ${coverage_lib})
add_test(NAME simple_tests.pub_sub COMMAND $<TARGET_FILE:test_pub_sub>)

# CLIENT / SERVER TESTS
#add_executable(test_client test_client.cpp)
#target_link_libraries(test_client simple-static ${coverage_lib})
#add_test(NAME simple_tests.client COMMAND $<TARGET_FILE:test_client>)

#add_executable(test_server test_server.cpp)
#target_link_libraries(test_server simple-static ${coverage_lib})
#add_test(NAME simple_tests.server COMMAND $<TARGET_FILE:test_server>)

add_executable(test_req_rep test_single_req_rep.cpp)
target_link_libraries(test_req_rep simple-static ${coverage_lib})
add_test(NAME simple_tests.req_rep COMMAND $<TARGET_FILE:test_req_rep>)

install(
TARGETS
  test_bool
  test_string
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
  test_req_rep
DESTINATION 
	bin
)
