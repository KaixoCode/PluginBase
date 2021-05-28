cmake_minimum_required (VERSION 3.0)
project (PluginBase)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(libs)

set(EB_SRC "${PluginBase_SOURCE_DIR}/")

file(GLOB_RECURSE PLUGINBASE_SOURCE
  "${EB_SRC}include/*.hpp"
)

add_library(PluginBase STATIC
  ${PLUGINBASE_SOURCE}
)

target_include_directories(PluginBase PUBLIC
  ${EB_SRC}libs/json/include
  ${EB_SRC}include/
)

set_target_properties(PluginBase PROPERTIES LINKER_LANGUAGE CXX)