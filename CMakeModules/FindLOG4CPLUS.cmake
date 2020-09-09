# - Find LOG4CPLUS
# Find the native LOG4CPLUS includes and library
#
#  LOG4CPLUS_INCLUDE_DIRS - where to find LOG4CPLUS.h, etc.
#  LOG4CPLUS_LIBRARIES   - List of libraries when using LOG4CPLUS.
#  LOG4CPLUS_FOUND       - True if LOG4CPLUS found.

# 判断是否已经包含LOG4CPLUS
#if (LOG4CPLUS_INCLUDE_DIR)
#    set(LOG4CPLUS_FIND_QUIETLY TRUE)
#endif ()

# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的LOG4CPLUS位于系统路径/LOG4CPLUS/文件夹下
find_path(LOG4CPLUS_INCLUDE_DIR
        NAMES log4cplus/logger.h
        # 可以通过以下命令来手动制定查找路径
        # PATHS /usr/local/include
#        PATH_SUFFIXES log4cplus
        DOC "LOG4CPLUS include directories"
        )
# 查找库文件位置
find_library(LOG4CPLUS_LIBRARY
        NAMES log4cplus
        DOC "LOG4CPLUS library"
        )

# 同时找到头文件位置和库文件位置时给相关变量赋值
if (LOG4CPLUS_INCLUDE_DIR AND LOG4CPLUS_LIBRARY)
    set(LOG4CPLUS_FOUND TRUE)
    set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_LIBRARY})
    set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIR})
    message(STATUS "log4cplus found")
else ()
    set(JSONCPP_FOUND FALSE)
    message(WARNING "log4cplus not found")
endif ()
if (LOG4CPLUS_INCLUDE_DIR)
    message(STATUS "found log4cplus include dir: ${LOG4CPLUS_INCLUDE_DIR}")
endif ()
if (LOG4CPLUS_LIBRARY)
    message(STATUS "found log4cplus libraries: ${LOG4CPLUS_LIBRARY}")
endif()
