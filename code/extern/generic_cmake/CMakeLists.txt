cmake_minimum_required(VERSION 3.2)

project(generic_cmake)

## header-only interface-library
add_library(          gnr_h INTERFACE)
target_include_directories(gnr_h INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

## individual libraries
add_library(          gnr_alloca "${CMAKE_CURRENT_LIST_DIR}/generic/alloca.cpp")
target_link_libraries(gnr_alloca INTERFACE gnr_h)

add_library(          gnr_anyfind "${CMAKE_CURRENT_LIST_DIR}/generic/anyfind.cpp")
target_link_libraries(gnr_anyfind INTERFACE gnr_h)

add_library(          gnr_callback "${CMAKE_CURRENT_LIST_DIR}/generic/callback.cpp")
target_link_libraries(gnr_callback INTERFACE gnr_h)

add_library(          gnr_cify "${CMAKE_CURRENT_LIST_DIR}/generic/cify.cpp")
target_link_libraries(gnr_cify INTERFACE gnr_h)

add_library(          gnr_compose "${CMAKE_CURRENT_LIST_DIR}/generic/compose.cpp")
target_link_libraries(gnr_compose INTERFACE gnr_h)

add_library(          gnr_coroutine "${CMAKE_CURRENT_LIST_DIR}/generic/coroutine.cpp")
target_link_libraries(gnr_coroutine INTERFACE gnr_h)

add_library(          gnr_forwarder "${CMAKE_CURRENT_LIST_DIR}/generic/forwarder.cpp")
target_link_libraries(gnr_forwarder INTERFACE gnr_h)

add_library(          gnr_lightptr "${CMAKE_CURRENT_LIST_DIR}/generic/lightptr.cpp")
target_link_libraries(gnr_lightptr INTERFACE gnr_h)

add_library(          gnr_many "${CMAKE_CURRENT_LIST_DIR}/generic/many.cpp")
target_link_libraries(gnr_many INTERFACE gnr_h)

add_library(          gnr_string "${CMAKE_CURRENT_LIST_DIR}/generic/string.cpp")
target_link_libraries(gnr_string INTERFACE gnr_h)

## gnr_libs links to all libs
add_library(          gnr_libs gnr_libs.cpp)
target_link_libraries(gnr_libs
                             gnr_alloca gnr_anyfind gnr_callback gnr_cify gnr_compose gnr_coroutine gnr_forwarder gnr_lightptr gnr_many gnr_string)

