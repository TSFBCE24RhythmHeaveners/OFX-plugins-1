cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# set the project name
project(AdjustRGB)
# get all cpp source files
file(GLOB SOURCES "src/*.cpp")
# for library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# OFX headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include-ofx)
# local headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include-local)

add_library(AdjustRGB SHARED ${SOURCES})
SET_TARGET_PROPERTIES(AdjustRGB PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(AdjustRGB PROPERTIES SUFFIX ".ofx")
target_link_libraries(AdjustRGB)

set_property(TARGET AdjustRGB PROPERTY CXX_STANDARD 14)
