cmake_minimum_required(VERSION 3.3)
project(simple_examples)

option(SIMPLE_BUILD_OPENCV_EXAMPLES "Build SIMPLE Examples that use OpenCV" FALSE)

if (${SIMPLE_BUILD_OPENCV_EXAMPLES})
  find_package(OpenCV 4 REQUIRED COMPONENTS opencv_core opencv_videoio opencv_highgui)
  add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")

  add_executable(example_image_publisher src/example_image_publisher.cpp)
  target_include_directories(example_image_publisher PRIVATE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(example_image_publisher simple-static opencv_core opencv_videoio)

  add_executable(example_image_subscriber src/example_image_subscriber.cpp)
  target_include_directories(example_image_subscriber PRIVATE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(example_image_subscriber simple-static opencv_core opencv_highgui)

  install(TARGETS example_image_publisher example_image_subscriber DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

add_executable(example_publisher src/example_publisher.cpp)
target_link_libraries(example_publisher simple-static)

add_executable(example_subscriber src/example_subscriber.cpp)
target_link_libraries(example_subscriber simple-static)

add_executable(example_client src/example_client.cpp)
target_link_libraries(example_client simple-static)

add_executable(example_server src/example_server.cpp)
target_link_libraries(example_server simple-static)

install(TARGETS example_publisher example_subscriber example_client example_server DESTINATION ${CMAKE_INSTALL_BINDIR})
