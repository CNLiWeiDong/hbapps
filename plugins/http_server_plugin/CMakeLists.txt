cmake_minimum_required(VERSION 3.5)
project(http_server_plugin)

file(GLOB HEADERS "include/hb/http_server_plugin/*.h"
        )

add_library(http_server_plugin
        common.cpp
        session_ssl.cpp
        session.cpp
        listener.cpp
        http_server_plugin.cpp
        http_server_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( http_server_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( http_server_plugin
        appbase
        hb
        thread_pool_plugin
        log_plugin
        )