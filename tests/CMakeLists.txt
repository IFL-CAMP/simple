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

#TESTS FOR WRAPPERS

add_executable(point_test point_test.cpp)
target_link_libraries(point_test simple)

add_executable(rotation_matrix_test rotation_matrix_test.cpp)
target_link_libraries(rotation_matrix_test simple)

add_executable(pose_test pose_test.cpp)
target_link_libraries(pose_test simple)

add_executable(quaternion_test quaternion_test.cpp)
target_link_libraries(quaternion_test simple)

install(
TARGETS 
  point_test
  pose_test
  rotation_matrix_test
  quaternion_test
DESTINATION 
	bin
)
