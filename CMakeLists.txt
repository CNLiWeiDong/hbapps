cmake_minimum_required(VERSION 3.5)
project(cpp-server)

set( CMAKE_CXX_STANDARD 17 )
#set constum find_package modele path.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
# 以后不使用，使用find_library 指定.a查找表态库 名字默认搜索动态库 如：curl
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a${CMAKE_FIND_LIBRARY_SUFFIXES}")
#set cmake test enable
enable_testing()
#New CXX_STANDARD and CXX_EXTENSIONS target properties may specify values which CMake uses to compute required compile options such as -std=c++11 or -std=gnu++11.
#The CMAKE_CXX_STANDARD and CMAKE_CXX_EXTENSIONS variables may be set to initialize the target properties.
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON)
#Enable/Disable output of compile commands during generation.If enabled, generates a compile_commands.json file containing the exact compiler calls
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
#use static lib
set( BUILD_SHARED_LIBS NO )
#编译静态库使用fPIC
add_compile_options(-fPIC)

if (UNIX AND APPLE)
    #Path used for searching by FIND_XXX(), with appropriate suffixes added.
    #list(APPEND CMAKE_INCLUDE_PATH "include_path")
    #list(APPEND CMAKE_LIBRARY_PATH "lib_path")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/llvm")
endif()

# detect operating system
message(STATUS "project is in ${CMAKE_SYSTEM_NAME} system")
if( APPLE )
    # Apple Specific Options Here
    set(darwin true CACHE INTERNAL "darwin flag" )
    add_definitions(-Ddarwin)
else( APPLE )
    set(linux true CACHE INTERNAL "linux flag" )
    add_definitions(-Dlinux)
endif( APPLE )
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(
            -D_DEBUG
            -DDEBUG
    )
    message(STATUS "project is in Debug modle")
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    add_definitions(
            -DNDEBUG
            -D_RELEASE
    )
    message(STATUS "project is in Release modle")
endif ()


message( STATUS ${CMAKE_GENERATOR})
# http://stackoverflow.com/a/18369825
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        message(FATAL_ERROR "GCC version must be at least 7.0!")
    endif()
    if ("${CMAKE_GENERATOR}" STREQUAL "Ninja")
        add_compile_options(-fdiagnostics-color=always)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        message(FATAL_ERROR "Clang version must be at least 5.0!")
    endif()
    if ("${CMAKE_GENERATOR}" STREQUAL "Ninja")
        add_compile_options(-fcolor-diagnostics)
    endif()
endif()

if(UNIX)
    if(APPLE)
        set(whole_archive_flag "-force_load")
        set(no_whole_archive_flag "")
    else()
        set(whole_archive_flag "--whole-archive")
        set(no_whole_archive_flag "--no-whole-archive")
    endif()
endif()

if( APPLE )
    # Apple Specific Options Here
    message( STATUS "Configuring project on OS X" )
    set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations" )
else( APPLE )
    # Linux Specific Options Here
    message( STATUS "Configuring project on Linux" )
    set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -Wall" )
    # FULL_STATIC_BUILD
    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

    if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
        if( CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.0.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.0.0 )
            set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-partial-specialization" )
        endif()
    endif()
endif( APPLE )


# add defaults for thread
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

#ZLIB rt dl 
# openssl need
IF(APPLE)
    find_package( ZLIB REQUIRED )
    find_library(security_framework Security) 
    find_library(corefoundation_framework CoreFoundation)
ELSE(APPLE)
    find_package( ZLIB )
    SET(rt_library rt )
    SET(pthread_library pthread)
    set(dl_library dl)
ENDIF(APPLE)
if( ZLIB_FOUND )
    MESSAGE( STATUS "zlib found" )
    add_definitions( -DHAS_ZLIB )
else()
    MESSAGE( STATUS "zlib not found" )
    set( ZLIB_LIBRARIES "" )
endif( ZLIB_FOUND )

#openssl
if ("${OPENSSL_ROOT_DIR}" STREQUAL "")
    if (NOT "$ENV{OPENSSL_ROOT_DIR}" STREQUAL "")
        set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})
        set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
    elseif (APPLE)
        set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
        set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl/include")
    elseif(UNIX AND NOT APPLE)
        set(OPENSSL_ROOT_DIR "/usr/include/openssl")
        set(OPENSSL_INCLUDE_DIR "/usr/include/openssl/include")
    else()
        message(FATAL_ERROR "openssl not found and don't know where to look, please specify OPENSSL_ROOT_DIR")
    endif()
endif()
SET(OPENSSL_CONF_TARGET )
IF(DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    SET (OPENSSL_CONF_TARGET ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ELSE()
    SET (OPENSSL_CONF_TARGET ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL ${OPENSSL_LIBRARIES}")

#boost
SET( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )
### Remove after Boost 1.70 CMake fixes are in place
set( Boost_NO_BOOST_CMAKE ON CACHE STRING "ON or OFF" )
find_package(Boost 1.67 REQUIRED COMPONENTS
        log_setup
        log
        date_time
        filesystem
        system
        thread
        regex
        program_options
        chrono
        unit_test_framework
        iostreams
        )
add_definitions(-DBOOST_ASIO_DISABLE_STD_EXPERIMENTAL_STRING_VIEW)
message(STATUS ${Boost_LIBRARIES})

add_subdirectory(common)
add_subdirectory(plugins)
add_subdirectory(apps)



##########编译选项 begin############
#SET( BUILD_SHARED_LIBS ON ) #所有库使用动态库
#add_compile_options(-fPIC) #编译静态库使用fPIC
#add_compile_options(-O3) #优化级别
#set(CMAKE_EXE_LINKER_FLAGS " -no-pie") #链接时不需要库是pic的
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1) #不使用c11库

#SET (CMAKE_C_FLAGS                "-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG          "-g")
#SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
#SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
##########编译选项 end############

#设置库的前缀
#SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
#设置库的后缀
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})
#find_path 和 find_library 时，会搜索一些默认的路径。
#当我们将一些lib安装在非默认搜索路径时，cmake就没法搜索到了。
#这是我们需要添加路径。
#set(CMAKE_INCLUDE_PATH "include_path")
#set(CMAKE_LIBRARY_PATH "lib_path")
#引用PkgConfig，可使用pkg查找库
#find_package(PkgConfig REQUIRED)






