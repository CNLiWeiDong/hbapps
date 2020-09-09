cmake_minimum_required(VERSION 3.5)
project(mysql_plugin)

set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS "include/hb/mysql_plugin/*.h"
        )

add_library(mysql_plugin
        mysql_plugin.cpp
        mysql_plugin_impl.cpp
        ${HEADERS}
        )
find_package(MYSQLPP REQUIRED)
find_package(MYSQLCLIENT REQUIRED)

target_include_directories( mysql_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        ${MYSQLPP_INCLUDE_DIR}
        ${MYSQLCLIENT_INCLUDE_DIR}
        )

target_link_libraries( mysql_plugin
        logging_plugin
        ${MYSQLPP_LIBRARY}
        ${MYSQLCLIENT_LIBRARY}
        appbase
        fc
        )