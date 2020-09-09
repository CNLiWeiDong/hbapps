cmake_minimum_required(VERSION 3.5)
project(monitor_price_plugin)

file(GLOB HEADERS "include/hb/monitor_price_plugin/*.h"
        )

add_library(monitor_price_plugin
        monitor_price_plugin.cpp
        monitor_price_plugin_impl.cpp
        monitor_targets.cpp
        monitor_singles.cpp
        monitor_pairs.cpp
        ${HEADERS}
        )

target_include_directories( monitor_price_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        )

target_link_libraries( monitor_price_plugin
        appbase
        fc
        logging_plugin
        send_mail_plugin
        )