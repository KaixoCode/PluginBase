cmake_minimum_required (VERSION 3.0)
project (EffectBase)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(libs)

set(EB_SRC "${EffectBase_SOURCE_DIR}/")

file(GLOB_RECURSE EFFECTBASE_SOURCE
  "${EB_SRC}include/*.hpp"
)

add_library(EffectBase STATIC
  ${EFFECTBASE_SOURCE}
)

target_include_directories(EffectBase PUBLIC
  ${EB_SRC}libs/json/include
  ${EB_SRC}include/
)

set_target_properties(EffectBase PROPERTIES LINKER_LANGUAGE CXX)