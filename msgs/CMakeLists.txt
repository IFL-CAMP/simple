cmake_minimum_required(VERSION 3.3)
project(simple_msgs)

# Find FlatBuffers and Flatc.
find_package(Flatbuffers REQUIRED)

find_program(FLATBUFFERS_FLATC_EXECUTABLE flatc
  /usr/local/bin
  /usr/bin
  "$ENV{PROGRAMFILES}/FlatBuffers/bin"
  "${Flatbuffers_DIR}/../../../bin"
  "${Flatbuffers_DIR}/../../tools/flatbuffers"
  NO_DEFAULT_PATH)

if(NOT FLATBUFFERS_FLATC_EXECUTABLE)
	message(FATAL_ERROR "Flatbuffers' flatc could not be found. Please set FLATBUFFERS_FLATC_EXECUTABLE manually!")
endif()

# CMake function that generates Flatbuffers headers from fbs files in the given directory.
function(flatbuffers_generate_c_headers NAME FBS_DIR OUTPUT_DIR)
  set(FLATC_OUTPUTS)
  file(GLOB FBS_FILES ${FBS_DIR}/*.fbs)
  foreach(FILE ${FBS_FILES})
    get_filename_component(FLATC_OUTPUT ${FILE} NAME_WE)
    set(FLATC_OUTPUT "${OUTPUT_DIR}/${FLATC_OUTPUT}_generated.h")
    list(APPEND FLATC_OUTPUTS ${FLATC_OUTPUT})
    set(EXECUTE_COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE} -c -o ${OUTPUT_DIR} ${FILE})
    execute_process(COMMAND ${EXECUTE_COMMAND})
  endforeach()
  set(${NAME}_OUTPUTS ${FLATC_OUTPUTS} PARENT_SCOPE)
endfunction()

###### SOURCE FILES

file(GLOB_RECURSE SIMPLE_MSGS_HEADERS "*.h" "*.hpp")

set(SIMPLE_MSGS_SRCS
  ${SIMPLE_MSGS_HEADERS}
  src/bool.cpp
  src/string.cpp
  src/header.cpp
  src/image.cpp
  src/point.cpp
  src/point_stamped.cpp
  src/quaternion.cpp
  src/quaternion_stamped.cpp
  src/pose.cpp
  src/pose_stamped.cpp
  src/rotation_matrix.cpp
  src/rotation_matrix_stamped.cpp
  src/homo_matrix.cpp
  src/homo_matrix_stamped.cpp
  )

# Generate the Flatbuffers headers.
flatbuffers_generate_c_headers(simple_msgs ${PROJECT_SOURCE_DIR}/fbs ${PROJECT_SOURCE_DIR}/include/simple_msgs/generated)

###### TARGETS

add_library(${PROJECT_NAME} STATIC ${SIMPLE_MSGS_SRCS})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/generated>
)

target_link_libraries(${PROJECT_NAME} PUBLIC flatbuffers::flatbuffers)

###### INSTALLATION

# Install the simple_msgs library in the desired folder.
install(TARGETS ${PROJECT_NAME} EXPORT "simpleTargets"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
