cmake_minimum_required(VERSION 3.5)
project(monitor_mail_plugin)

file(GLOB HEADERS "include/hb/monitor_mail_plugin/*.h"
        )

add_library(monitor_mail_plugin
        monitor_mail_plugin.cpp
        monitor_mail_plugin_impl.cpp
        ${HEADERS}
        )

target_include_directories( monitor_mail_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( monitor_mail_plugin
        appbase
        hb
        log_plugin
        send_mail_plugin
        )
