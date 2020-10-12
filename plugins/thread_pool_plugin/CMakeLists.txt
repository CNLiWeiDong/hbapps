cmake_minimum_required(VERSION 3.5)
project(thread_pool_plugin)

file(GLOB HEADERS "include/hb/thread_pool_plugin/*.h"
        )

add_library(thread_pool_plugin
        thread_pool_plugin.cpp
        thread_pool_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( thread_pool_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( thread_pool_plugin
        log_plugin
        appbase
        )