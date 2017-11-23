#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Tries to find Flatbuffers headers and libraries.
#
# Usage of this module as follows:
#
#  find_package(FlatBuffers)
#
#
# This module defines
#  FLATBUFFERS_INCLUDE_DIR - directory containing headers
#  FLATBUFFERS_LIBS - the flatbuffers library
#  FLATBUFFERS_FLATC_EXECUTABLE - the flatc executable
#  FLATBUFFERS_FOUND - whether flatbuffers has been found


find_path( FLATBUFFERS_INCLUDE_DIR NAMES flatbuffers/flatbuffers.h )
find_library( FLATBUFFERS_LIBS NAMES flatbuffers )

find_program(FLATBUFFERS_FLATC_EXECUTABLE flatc
  /usr/local/bin
  /usr/bin
  NO_DEFAULT_PATH
)

if (FLATBUFFERS_INCLUDE_DIR AND FLATBUFFERS_LIBS AND FLATBUFFERS_FLATC_EXECUTABLE)
  set(FLATBUFFERS_FOUND TRUE)
else ()
  set(FLATBUFFERS_FOUND FALSE)
endif ()

if (FLATBUFFERS_FOUND)
  if (NOT FlatBuffers_FIND_QUIETLY)
    message(STATUS "Found the Flatbuffers library: ${FLATBUFFERS_LIBS}")
  endif ()
else ()
  if (NOT Flatbuffers_FIND_QUIETLY)
    if (FLATBUFFERS_INCLUDE_DIR AND FLATBUFFERS_LIBS)
      set(FLATBUFFERS_ERR_MSG "Could not find the flatc executable. Please provide its path.")
    else()
      set(FLATBUFFERS_ERR_MSG "Could not find the Flatbuffers library. Please provide its path.")
    endif()
    if (FlatBuffers_FIND_REQUIRED)
      message(FATAL_ERROR "${FLATBUFFERS_ERR_MSG}")
      message(I AM HERE)
    else ()
      message(STATUS "${FLATBUFFERS_ERR_MSG}")
    endif ()
  endif ()
endif ()

if(FLATBUFFERS_FOUND)
  function(FLATBUFFERS_GENERATE_C_HEADERS NAME FBS_DIR OUTPUT_DIR)
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
endif()