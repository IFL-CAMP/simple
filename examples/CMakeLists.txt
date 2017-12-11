cmake_minimum_required(VERSION 3.5)
project(simple_examples)

set(pose_pub "pose_pub_example")
set(pose_sub "pose_sub_example")

set(simple_examples_targets ${pose_pub} ${pose_sub})

add_executable(${pose_pub} pose_publisher.cpp)
add_executable(${pose_sub} pose_subscriber.cpp)

target_link_libraries(${pose_pub} simple)
target_link_libraries(${pose_sub} simple)

install(TARGETS ${simple_examples_targets} DESTINATION ${CMAKE_INSTALL_BINDIR})
