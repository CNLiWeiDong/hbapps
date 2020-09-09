cmake_minimum_required(VERSION 3.5)
project(grid_db_test)

set(CMAKE_CXX_STANDARD 14)


add_executable(grid_db_test main.cpp)

target_include_directories(grid_db_test
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries( grid_db_test
        grid_db_plugin
        )

add_test(NAME grid_db_test COMMAND grid_db_test)