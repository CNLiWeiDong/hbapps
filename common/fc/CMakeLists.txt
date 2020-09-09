cmake_minimum_required(VERSION 3.5)
project(fc)

set(fc_sources
        src/types/bignum.cpp
        src/compress/zlib.cpp
        src/crypto/_digest_common.cpp
        src/crypto/aes.cpp
        src/crypto/base58.cpp
        src/crypto/base64.cpp
        src/crypto/hex.cpp
        src/crypto/md5.cpp
        src/crypto/openssl.cpp
        src/crypto/rand.cpp
        src/crypto/ripemd160.cpp
        src/crypto/rsa.cpp
        src/crypto/sha1.cpp
        src/crypto/sha256.cpp
        src/crypto/sha512.cpp
        src/logging/logging.cpp
        src/http/http.cpp
        src/http/session.cpp
        src/https/https.cpp
        src/https/session.cpp
        )

file( GLOB_RECURSE fc_headers ${CMAKE_CURRENT_SOURCE_DIR} *.hpp *.h )
set( sources
        ${fc_sources}
        )
list(APPEND sources ${fc_headers})
add_library(fc ${sources})

target_include_directories(fc
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
        )
target_link_libraries( fc PUBLIC
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${rt_library}
        ${security_framework}
        ${corefoundation_framework}
        ${pthread_library}
        ${dl_library}
        )

IF(NOT DEFINED SKIP_FC_TESTS)
    add_subdirectory( test )
ENDIF()



