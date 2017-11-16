cmake_minimum_required(VERSION 3.5)
project(simple-serverNoReply)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} simple)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)