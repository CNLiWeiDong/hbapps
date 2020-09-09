# - Find MYSQLCLIENT
# Find the native MYSQLCLIENT includes and library
#
#  MYSQLCLIENT_INCLUDE_DIRS - where to find mysql++.h.
#  MYSQLCLIENT_LIBRARIES   - List of libraries when using MYSQLCLIENT.
#  MYSQLCLIENT_FOUND       - True if JSONCPP found.

# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的JSONCPP位于系统路径/JSONCPP/文件夹下
find_path(MYSQLCLIENT_INCLUDE_DIR
        NAMES mysql.h
        PATHS /usr/local/opt/mysql/include/mysql /usr/include/mysql
        DOC "MYSQLCLIENT include directories"
        )
# 查找库文件位置
find_library(MYSQLCLIENT_LIBRARY
        NAMES libmysqlclient.a mysqlclient
        PATHS /usr/local/opt/mysql/lib /usr/lib/x86_64-linux-gnu
        DOC "MYSQLCLIENT library"
        )
# 同时找到头文件位置和库文件位置时给相关变量赋值
if (MYSQLCLIENT_INCLUDE_DIR AND MYSQLCLIENT_LIBRARY)
    set(MYSQLCLIENT_FOUND TRUE)
    set(MYSQLCLIENT_LIBRARIES ${MYSQLCLIENT_LIBRARY})
    set(MYSQLCLIENT_INCLUDE_DIRS ${MYSQLCLIENT_INCLUDE_DIR})
    message(STATUS "MYSQLCLIENT found")
else ()
    set(MYSQLCLIENT_FOUND FALSE)
    message(WARNING "MYSQLCLIENT not found")
endif ()
if (MYSQLCLIENT_INCLUDE_DIR)
    message(STATUS "found MYSQLCLIENT include dir: ${MYSQLCLIENT_INCLUDE_DIR}")
endif ()
if (MYSQLCLIENT_LIBRARY)
    message(STATUS "found MYSQLCLIENT libraries: ${MYSQLCLIENT_LIBRARY}")
endif()
