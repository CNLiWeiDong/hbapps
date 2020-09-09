# - Find CryptoPP
# Find the native CryptoPP includes and library
#
#  CryptoPP_INCLUDE_DIRS - where to find mysql++.h.
#  CryptoPP_LIBRARIES   - List of libraries when using CryptoPP.
#  CryptoPP_FOUND       - True if CryptoPP found.


# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的CryptoPP位于系统路径/CryptoPP/文件夹下
find_path(CryptoPP_INCLUDE_DIR
        NAMES cryptopp/aes.h
        DOC "CryptoPP include directories"
        )
# 查找库文件位置
find_library(CryptoPP_LIBRARY
        NAMES libcrypto++.a crypto++
        PATHS /usr/local/opt/mysql++/lib /usr/opt/mysql++/lib
        DOC "CryptoPP library"
        )
# 同时找到头文件位置和库文件位置时给相关变量赋值
if (CryptoPP_INCLUDE_DIR AND CryptoPP_LIBRARY)
    set(CryptoPP_FOUND TRUE)
    set(CryptoPP_LIBRARIES ${CryptoPP_LIBRARY})
    set(CryptoPP_INCLUDE_DIRS ${CryptoPP_INCLUDE_DIR})
    message(STATUS "CryptoPP found")
else ()
    set(CryptoPP_FOUND FALSE)
    message(WARNING "CryptoPP not found")
endif ()
if (CryptoPP_INCLUDE_DIR)
    message(STATUS "found CryptoPP include dir: ${CryptoPP_INCLUDE_DIR}")
endif ()
if (CryptoPP_LIBRARY)
    message(STATUS "found CryptoPP libraries: ${CryptoPP_LIBRARY}")
endif()
