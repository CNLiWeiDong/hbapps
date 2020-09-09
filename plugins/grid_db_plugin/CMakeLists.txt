cmake_minimum_required(VERSION 3.5)
project(grid_db_plugin)

set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS "include/hb/grid_db_plugin/*.h"
        )

add_library(grid_db_plugin
        grid_db_plugin.cpp
        grid_db_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( grid_db_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( grid_db_plugin
        appbase
        fc
        logging_plugin
        mysql_plugin
        )
add_subdirectory(test)
