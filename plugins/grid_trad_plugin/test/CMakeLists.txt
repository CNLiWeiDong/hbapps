cmake_minimum_required(VERSION 3.5)
project(grid_trad_test)

set(CMAKE_CXX_STANDARD 14)


add_executable(grid_trad_test main.cpp)

target_include_directories(grid_trad_test
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries( grid_trad_test
        grid_trad_plugin
        )

add_test(NAME grid_trad_test COMMAND grid_trad_test)