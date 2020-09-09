cmake_minimum_required(VERSION 3.5)
project(logging_plugin)

file(GLOB HEADERS "include/hb/logging_plugin/*.h"
        )

add_library(logging_plugin
        logging_plugin.cpp
        ${HEADERS}
        )

target_include_directories( logging_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( logging_plugin
        fc
        appbase
        )