add_subdirectory(override_new)

function(create_symlink target_exe_name)
  add_custom_command(
    TARGET     ${target_exe_name}
    POST_BUILD
    COMMAND   "${CMAKE_COMMAND}" "-E" "create_symlink" "${CMAKE_CURRENT_BINARY_DIR}/${target_exe_name}" "${CMAKE_BINARY_DIR}/${target_exe_name}"
    )
endfunction()


add_executable(       go main.cpp)
target_link_libraries(go gnr)
create_symlink(       go)

add_executable(       forwarder_ex1 forwarder_ex1.cpp)
target_link_libraries(forwarder_ex1 gnr override_new)
create_symlink(       forwarder_ex1)

add_executable(       forwarder_ex2 forwarder_ex2.cpp)
target_link_libraries(forwarder_ex2 gnr override_new)
create_symlink(       forwarder_ex2)

add_executable(       test_override_new EXCLUDE_FROM_ALL test_override_new.cpp)
target_link_libraries(test_override_new override_new)
create_symlink(       test_override_new)
