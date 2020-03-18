cmake_minimum_required(VERSION 3.3)
project(simple_msgs)

# Force the use of the static version of protobuf.
set(Protobuf_USE_STATIC_LIBS ON)

if(DEFINED Protobuf_DIR)
  set(CMAKE_PREFIX_PATH "${Protobuf_DIR}" "${CMAKE_PREFIX_PATH}")
endif()

# Find protobuf and protoc,
find_package(Protobuf REQUIRED)

###### PROTOBUF ######

# Output directory for the generated proto messages.
set(PROTO_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")
# Create the output directory.
file(MAKE_DIRECTORY ${PROTO_OUTPUT_PATH})

# Languages for which proto messages are generated.
# By default cpp messages are always generated.
list(APPEND PROTO_LANGUAGE_GENERATION "cpp")

# CMake function that generates Protobuf messages from proto files in the given directory.
function(generate_protobuf_messages PROTO_DIR OUTPUT_DIR LANGUAGE_GENERATOR)
  # Gather all proto files.
  file(GLOB PROTO_FILES ${PROTO_DIR}/*.proto)
  # Creates the flags for the protoc command.
  list(APPEND PROTOC_FLAGS "--proto_path=${PROTO_DIR}")
  list(APPEND PROTOC_FLAGS "--${LANGUAGE_GENERATOR}_out=${OUTPUT_DIR}")
  # Execute the protobuf compiler and generates the appropriate messages.
  set(EXECUTE_COMMAND ${Protobuf_PROTOC_EXECUTABLE} ${PROTOC_FLAGS} ${PROTO_FILES})
  execute_process(COMMAND ${EXECUTE_COMMAND})
endfunction()

# Generate the proto messages for each selected language.
foreach(LANGUAGE ${PROTO_LANGUAGE_GENERATION})
  generate_protobuf_messages(${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_OUTPUT_PATH} ${LANGUAGE})
endforeach()

# CPP - Collect all generated heades and source files.
file(GLOB PROTO_CPP_HEADERS ${PROTO_OUTPUT_PATH}/*.pb.h)
file(GLOB PROTO_CPP_SOURCES ${PROTO_OUTPUT_PATH}/*.pb.cc)

###### TARGETS ######
add_library(${PROJECT_NAME} STATIC ${PROTO_CPP_SOURCES} ${PROTO_CPP_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>
  $<INSTALL_INTERFACE:include>
  )

target_compile_definitions(${PROJECT_NAME} PUBLIC SIMPLE_STATIC_DEFINE)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})

###### INSTALLATION ######
install(TARGETS ${PROJECT_NAME} EXPORT "simpleTargets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
install(DIRECTORY ${PROTO_OUTPUT_PATH} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
