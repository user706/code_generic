find_package(Boost 1.55 REQUIRED)

add_library(base INTERFACE)

target_include_directories(base
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(base
  INTERFACE
    Boost::boost)

file(GLOB HEADER *.hpp *.h)

add_executable(memfun_benchmark
  ${HEADER}
  ${CMAKE_CURRENT_SOURCE_DIR}/memfun_benchmark.cpp)

target_link_libraries(memfun_benchmark
  PUBLIC
    base)

create_symlink(       memfun_benchmark)
