cmake_minimum_required(VERSION 3.5)
project(simple_msgs)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER simple.proto)

add_library(${PROJECT_NAME} STATIC ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARY})
