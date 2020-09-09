# - Find JSONCPP
# Find the native JSONCPP includes and library
#
#  JSONCPP_INCLUDE_DIRS - where to find JSONCPP.h, etc.
#  JSONCPP_LIBRARIES   - List of libraries when using JSONCPP.
#  JSONCPP_FOUND       - True if JSONCPP found.


# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的JSONCPP位于系统路径/JSONCPP/文件夹下
find_path(JSONCPP_INCLUDE_DIR
        NAMES json/json.h
        # 可以通过以下命令来手动制定查找路径
        PATHS /usr/include/jsoncpp /usr/local/include
#        PATH_SUFFIXES jsoncpp
        DOC "JSONCPP include directories"
        )
# 查找库文件位置
find_library(JSONCPP_LIBRARY
        NAMES jsoncpp
        DOC "JSONCPP library"
        )

# 同时找到头文件位置和库文件位置时给相关变量赋值
if (JSONCPP_INCLUDE_DIR AND JSONCPP_LIBRARY)
    set(JSONCPP_FOUND TRUE)
    set(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
    set(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})
    message(STATUS "jsoncpp found")
else ()
    set(JSONCPP_FOUND FALSE)
    message(WARNING "jsoncpp not found")
endif ()
if (JSONCPP_INCLUDE_DIR)
    message(STATUS "found jsoncpp include dir: ${JSONCPP_INCLUDE_DIR}")
endif ()
if (JSONCPP_LIBRARY)
    message(STATUS "found jsoncpp libraries: ${JSONCPP_LIBRARY}")
endif()
