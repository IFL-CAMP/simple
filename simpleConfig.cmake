# - Config file for the simple package
include(CMakeFindDependencyMacro)
find_dependency(Threads)
include("${CMAKE_CURRENT_LIST_DIR}/simpleTargets.cmake")