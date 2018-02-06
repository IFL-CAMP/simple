cmake_minimum_required(VERSION 3.5)
project(simple_examples)

option(BUILD_EXAMPLES_WITH_OPENCV "Build SIMPLE Examples that use OpenCV" FALSE)

if (${BUILD_EXAMPLES_WITH_OPENCV})
  find_package(OpenCV REQUIRED)
  include_directories(${OpenCV_INCLUDE_DIRS})
  set(LIBRARIES simple-static ${OpenCV_LIBS})
  add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")

  set(img_pub_example "img_pub_example")
  set(img_sub_example "img_sub_example")
  add_executable(example_image_publisher src/example_image_publisher.cpp)
  add_executable(example_image_subscriber src/example_image_subscriber.cpp)
  target_link_libraries(example_image_publisher ${LIBRARIES})
  target_link_libraries(example_image_subscriber ${LIBRARIES})
else()
  set(LIBRARIES simple-static)
endif()

add_executable(example_publisher src/example_publisher.cpp)
add_executable(example_subscriber src/example_subscriber.cpp)
add_executable(example_client src/example_client.cpp)
add_executable(example_server src/example_server.cpp)

target_link_libraries(example_publisher ${LIBRARIES})
target_link_libraries(example_subscriber ${LIBRARIES})
target_link_libraries(example_client ${LIBRARIES})
target_link_libraries(example_server ${LIBRARIES})

set(simple_examples_targets example_publisher example_subscriber example_image_publisher example_image_subscriber example_client example_server)
install(TARGETS ${simple_examples_targets} DESTINATION ${CMAKE_INSTALL_BINDIR})
