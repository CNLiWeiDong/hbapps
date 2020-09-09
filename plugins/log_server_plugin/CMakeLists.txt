cmake_minimum_required(VERSION 3.5)
project(log_server_plugin)

file(GLOB HEADERS "include/hb/log_server_plugin/*.h"
        )

add_library(log_server_plugin
        log_server_plugin.cpp
        log_server_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( log_server_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( log_server_plugin
        appbase
        fc
        logging_plugin
        )