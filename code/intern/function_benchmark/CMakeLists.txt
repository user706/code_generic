find_package(Boost 1.55 REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB HEADER *.hpp *.h)

add_executable(function_benchmark
  ${HEADER}
  ${CMAKE_CURRENT_SOURCE_DIR}/function_benchmark.cpp)

target_link_libraries(function_benchmark
  PUBLIC
  Boost::boost
  gnr
  override_new
 )

create_symlink(       function_benchmark)
