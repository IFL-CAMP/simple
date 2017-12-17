cmake_minimum_required(VERSION 3.5)
project(simple_examples)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(LIBRARIES simple ${OpenCV_LIBS})

add_definitions( -DDATA_DIR="${PROJECT_SOURCE_DIR}/data/"  )

set(pub_example "pub_example")
set(sub_example "sub_example")
set(img_pub_example "img_pub_example")
set(img_sub_example "img_sub_example")
set(client_example "client_example")
set(server_example "server_example")

set(simple_examples_targets ${pub_example} ${sub_example} ${img_pub_example} ${img_sub_example} ${client_example} ${server_example})

add_executable(${pub_example} publisher.cpp)
add_executable(${sub_example} subscriber.cpp)
add_executable(${img_pub_example} image_publisher.cpp)
add_executable(${img_sub_example} image_subscriber.cpp)
add_executable(${client_example} client.cpp)
add_executable(${server_example} server.cpp)

target_link_libraries(${pub_example} ${LIBRARIES})
target_link_libraries(${sub_example} ${LIBRARIES})
target_link_libraries(${img_pub_example} ${LIBRARIES})
target_link_libraries(${img_sub_example} ${LIBRARIES})
target_link_libraries(${client_example} ${LIBRARIES})
target_link_libraries(${server_example} ${LIBRARIES})

install(TARGETS ${simple_examples_targets} DESTINATION ${CMAKE_INSTALL_BINDIR})
