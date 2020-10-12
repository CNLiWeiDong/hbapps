cmake_minimum_required(VERSION 3.5)
project(trader)

set(CMAKE_CXX_STANDARD 14)


add_executable(trader trader.cpp)

target_include_directories(trader
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries(trader
        PRIVATE appbase
        PRIVATE ${Boost_LIBRARIES} #boost test
        PRIVATE -Wl,${whole_archive_flag} log_plugin                        -Wl,${no_whole_archive_flag}
        PRIVATE -Wl,${whole_archive_flag} grid_trad_plugin                      -Wl,${no_whole_archive_flag}
        )