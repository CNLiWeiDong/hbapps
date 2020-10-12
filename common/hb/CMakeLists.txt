cmake_minimum_required(VERSION 3.5)
project(hb)

set(hb_sources
        src/zip/zlib.cpp
        src/log/log.cpp
        src/error/exception.cpp
        src/http/http.cpp
        src/http/session.cpp
        src/https/https.cpp
        src/https/session.cpp
        src/crypto/aes.cpp
        src/crypto/hex.cpp
        src/crypto/base.cpp
        src/crypto/sha.cpp
        src/crypto/md5.cpp
        src/crypto/rsa.cpp
        )

file( GLOB_RECURSE hb_headers ${CMAKE_CURRENT_SOURCE_DIR} *.hpp *.h )
set( sources
        ${hb_sources}
        )
list(APPEND sources ${hb_headers})
add_library(hb ${sources})

find_package(CryptoPP REQUIRED)

target_include_directories(hb
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
        ${CryptoPP_INCLUDE_DIR}
        )
target_link_libraries( hb PUBLIC
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${CryptoPP_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${rt_library}
        ${security_framework}
        ${corefoundation_framework}
        ${pthread_library}
        ${dl_library}
        )

IF(NOT DEFINED SKIP_HB_TESTS)
    add_subdirectory( test )
ENDIF()



