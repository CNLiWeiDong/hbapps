cmake_minimum_required(VERSION 3.5)
project(grid_trad_plugin)

file(GLOB HEADERS "include/hb/grid_trad_plugin/*.h"
        )

add_library(grid_trad_plugin
        grid_trad_plugin.cpp
        grid_trad_plugin_impl.cpp
        delvier_order.cpp
        new_order.cpp
        ${HEADERS}
        )

target_include_directories( grid_trad_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( grid_trad_plugin
        appbase
        hb
        log_plugin
        grid_db_plugin
        send_mail_plugin
        trad_api_plugin
        )

add_subdirectory(test)