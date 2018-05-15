cmake_minimum_required(VERSION 3.2)

project(generic_cmake)


if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    message("setting CMAKE_CXX_STANDARD to ${CMAKE_CXX_STANDARD}")
endif()

if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
    message("setting CMAKE_CXX_EXTENSIONS to ${CMAKE_CXX_EXTENSIONS}")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("setting CMAKE_BUILD_TYPE to ${CMAKE_BUILD_TYPE}")
endif()


if ( CMAKE_COMPILER_IS_GNUCC )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
endif()

find_package (Git REQUIRED)

execute_process (
  COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(code)
