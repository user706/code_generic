cmake_minimum_required(VERSION 3.2)

project(generic_cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package (Git REQUIRED)

execute_process (
  COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(code)
