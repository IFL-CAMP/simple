cmake_minimum_required(VERSION 3.5)
project(simple_examples)

set(pub_example "pub_example")
set(sub_example "sub_example")
set(client_example "client_example")
set(server_example "server_example")

set(simple_examples_targets ${pub_example} ${sub_example} ${client_example} ${server_example})

add_executable(${pub_example} publisher.cpp)
add_executable(${sub_example} subscriber.cpp)
add_executable(${client_example} client.cpp)
add_executable(${server_example} server.cpp)

target_link_libraries(${pub_example} simple)
target_link_libraries(${sub_example} simple)
target_link_libraries(${client_example} simple)
target_link_libraries(${server_example} simple)

install(TARGETS ${simple_examples_targets} DESTINATION ${CMAKE_INSTALL_BINDIR})
