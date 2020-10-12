cmake_minimum_required(VERSION 3.5)
project(aes_encrypt)

set(CMAKE_CXX_STANDARD 14)


add_executable(aes_encrypt main.cpp)

target_include_directories(aes_encrypt
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries(aes_encrypt
        PRIVATE hb
        )