cmake_minimum_required(VERSION 3.5)
project(log_server)

set(CMAKE_CXX_STANDARD 14)


add_executable(log_server log_server.cpp)

target_include_directories(log_server
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries(log_server
        PRIVATE appbase
        PRIVATE ${Boost_LIBRARIES} #boost test
        PRIVATE -Wl,${whole_archive_flag} log_plugin                        -Wl,${no_whole_archive_flag}
        PRIVATE -Wl,${whole_archive_flag} log_server_plugin                     -Wl,${no_whole_archive_flag}
        )