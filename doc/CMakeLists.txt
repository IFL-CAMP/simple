CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

# add a target to generate API documentation with Doxygen
find_package(Doxygen REQUIRED)

if(${DOXYGEN_FOUND})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(DOXYGEN ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
else()
  message("Doxygen needs to be installed to generate the Doxygen documentation.")
endif()
