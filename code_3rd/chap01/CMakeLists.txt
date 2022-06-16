# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(auto_multi_target LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O2")

file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${src_files})
    get_filename_component(exe_file ${src_file} NAME_WE)
    add_executable(${exe_file} ${src_file})
    target_link_libraries(${exe_file} glut GL GLU m)
ENDFOREACH()

