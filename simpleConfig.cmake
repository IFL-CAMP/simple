# - Config file for the simple package
include(CMakeFindDependencyMacro)
find_dependency(ZeroMQ)
find_dependency(FlatBuffers)
include("${CMAKE_CURRENT_LIST_DIR}/simpleTargets.cmake")





