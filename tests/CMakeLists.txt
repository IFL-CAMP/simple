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
add_executable(test_client test_client.cpp)
target_link_libraries(test_client simple-static ${coverage_lib})
add_test(NAME simple_tests.client COMMAND $<TARGET_FILE:test_client>)

add_executable(test_server test_server.cpp)
target_link_libraries(test_server simple-static ${coverage_lib})
add_test(NAME simple_tests.server COMMAND $<TARGET_FILE:test_server>)

add_executable(test_req_rep test_req_rep.cpp)
target_link_libraries(test_req_rep simple-static ${coverage_lib})
add_test(NAME simple_tests.req_rep COMMAND $<TARGET_FILE:test_req_rep>)

# CONTEXT_MANAGER TESTS
add_executable(test_context_manager test_context_manager.cpp)
target_link_libraries(test_context_manager simple ${coverage_lib})
add_test(NAME simple_tests.context_manager COMMAND $<TARGET_FILE:test_context_manager>)

install(
  TARGETS
  test_publisher
  test_subscriber
  test_pub_sub
  test_client
  test_server
  test_req_rep
  test_context_manager
  DESTINATION ${CMAKE_INSTALL_BINDIR})
