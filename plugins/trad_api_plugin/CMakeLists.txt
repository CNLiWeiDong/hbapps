cmake_minimum_required(VERSION 3.5)
project(trad_api_plugin)

file(GLOB HEADERS "include/hb/trad_api_plugin/*.h"
        )

add_library(trad_api_plugin
        trad_api_plugin.cpp
        trad_api_plugin_impl.cpp
        ${HEADERS}
        )
# curl自带的
# set(CURL_STATICLIB ON)
# find_package(CURL)
# if (CURL_INCLUDE_DIRS)
#     message(STATUS "found CURL include dir: ${CURL_INCLUDE_DIRS}")
# endif ()
# if (CURL_LIBRARIES)
#     message(STATUS "found CURL libraries: ${CURL_LIBRARIES}")
# endif()

# 自己编写的
find_package(Curl)

target_include_directories( trad_api_plugin
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
        ${Curl_INCLUDE_DIRS}
        )

target_link_libraries( trad_api_plugin
        ${Curl_LIBRARIES}
        appbase
        fc
        logging_plugin
        # idn
        )

add_subdirectory(test)
