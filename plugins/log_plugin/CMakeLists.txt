cmake_minimum_required(VERSION 3.5)
project(log_plugin)

file(GLOB HEADERS "include/hb/log_plugin/*.h"
        )

add_library(log_plugin
        log_plugin.cpp
        ${HEADERS}
        )

target_include_directories( log_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( log_plugin
        hb
        appbase
        )