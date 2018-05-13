cmake_minimum_required(VERSION 3.2)

project(generic_cmake)

## header-only interface-library
add_library(               gnr INTERFACE)
target_include_directories(gnr INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
