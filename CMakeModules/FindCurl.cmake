# - Find Curl
# Find the native Curl includes and library
#
#  Curl_INCLUDE_DIRS - where to find mysql++.h.
#  Curl_LIBRARIES   - List of libraries when using Curl.
#  Curl_FOUND       - True if JSONCPP found.

# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的JSONCPP位于系统路径/JSONCPP/文件夹下
find_path(Curl_INCLUDE_DIR
        NAMES curl/curl.h
        DOC "culr include directories"
        )
# 查找库文件位置
find_library(Curl_LIBRARY
        NAMES curl
        DOC "Curl library"
        )
# 同时找到头文件位置和库文件位置时给相关变量赋值
if (Curl_INCLUDE_DIR AND Curl_LIBRARY)
    set(Curl_FOUND TRUE)
    set(Curl_LIBRARIES ${Curl_LIBRARY})
    set(Curl_INCLUDE_DIRS ${Curl_INCLUDE_DIR})
    message(STATUS "Curl found")
else ()
    set(Curl_FOUND FALSE)
    message(WARNING "Curl not found")
endif ()
if (Curl_INCLUDE_DIR)
    message(STATUS "found Curl include dir: ${Curl_INCLUDE_DIR}")
endif ()
if (Curl_LIBRARY)
    message(STATUS "found Curl libraries: ${Curl_LIBRARY}")
endif()
