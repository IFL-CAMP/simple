cmake_minimum_required(VERSION 3.5)
project(simple-serverPosition)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} simple)
