# - Find MYSQLPP
# Find the native MYSQLPP includes and library
#
#  MYSQLPP_INCLUDE_DIRS - where to find mysql++.h.
#  MYSQLPP_LIBRARIES   - List of libraries when using MYSQLPP.
#  MYSQLPP_FOUND       - True if MYSQLPP found.


# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的MYSQLPP位于系统路径/MYSQLPP/文件夹下
find_path(MYSQLPP_INCLUDE_DIR
        NAMES mysql++.h
        PATHS /usr/local/opt/mysql++/include/mysql++ /usr/opt/mysql++/include/mysql++
        DOC "MYSQLPP include directories"
        )
# 查找库文件位置
find_library(MYSQLPP_LIBRARY
        NAMES mysqlpp libmysqlpp.a
        PATHS /usr/local/opt/mysql++/lib /usr/opt/mysql++/lib
        DOC "MYSQLPP library"
        )
# 同时找到头文件位置和库文件位置时给相关变量赋值
if (MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARY)
    set(MYSQLPP_FOUND TRUE)
    set(MYSQLPP_LIBRARIES ${MYSQLPP_LIBRARY})
    set(MYSQLPP_INCLUDE_DIRS ${MYSQLPP_INCLUDE_DIR})
    message(STATUS "MYSQLPP found")
else ()
    set(MYSQLPP_FOUND FALSE)
    message(WARNING "MYSQLPP not found")
endif ()
if (MYSQLPP_INCLUDE_DIR)
    message(STATUS "found MYSQLPP include dir: ${MYSQLPP_INCLUDE_DIR}")
endif ()
if (MYSQLPP_LIBRARY)
    message(STATUS "found MYSQLPP libraries: ${MYSQLPP_LIBRARY}")
endif()
