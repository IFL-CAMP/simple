# - Config file for the simple package
# It defines the following variables
#  SIMPLE_INCLUDE_DIRS - include directories for simple
#  SIMPLE_LIBRARIES    - libraries to link against

# Compute paths
get_filename_component(SIMPLE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SIMPLE_INCLUDE_DIRS "C:/Users/Jack/Repo/simple;C:/Users/Jack/Repo/simple/build2017")

# Our library dependencies (contains definitions for IMPORTED targets)
include("${SIMPLE_CMAKE_DIR}/simpleTargets.cmake")

# These are IMPORTED targets created by simpleTargets.cmake
set(SIMPLE_LIBRARIES simple)
