#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "simple_msgs" for configuration "RelWithDebInfo"
set_property(TARGET simple_msgs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(simple_msgs PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/simple_msgs.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS simple_msgs )
list(APPEND _IMPORT_CHECK_FILES_FOR_simple_msgs "${_IMPORT_PREFIX}/bin/simple_msgs.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
