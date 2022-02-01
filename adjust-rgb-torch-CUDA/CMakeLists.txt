cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# set the project name
project(rgb)

# set torch libray
# set(CMAKE_PREFIX_PATH "P:/mattc/source/libtorch-cuda-10-2/share/cmake/Torch")
# set(CMAKE_PREFIX_PATH "P:/mattc/source/libtorch/share/cmake/Torch")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# get all cpp source files
file(GLOB SOURCES "src/*.cpp")
# for library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# OFX headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include-ofx)
# local headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include-local)

add_library(rgb SHARED ${SOURCES})
SET_TARGET_PROPERTIES(rgb PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(rgb PROPERTIES SUFFIX ".ofx")

# target_link_libraries(rgb PRIVATE "${TORCH_LIBRARIES} -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
target_link_libraries(rgb PRIVATE "${TORCH_LIBRARIES}")

target_link_libraries(rgb)

set_property(TARGET rgb PROPERTY CXX_STANDARD 14)

# for windows to link libraries
file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
add_custom_command(TARGET rgb
             POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy_if_different
             ${TORCH_DLLS}
           $<TARGET_FILE_DIR:rgb>)
