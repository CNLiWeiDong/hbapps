cmake_minimum_required(VERSION 3.5)
project(trad_api_test)

set(CMAKE_CXX_STANDARD 14)


add_executable(trad_api_test main.cpp)

target_include_directories(trad_api_test
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries( trad_api_test
        trad_api_plugin
        )

add_test(NAME trad_api_test COMMAND trad_api_test)