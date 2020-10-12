cmake_minimum_required(VERSION 3.5)
project(monitor_mail)

set(CMAKE_CXX_STANDARD 14)


add_executable(monitor_mail monitor_mail.cpp)

target_include_directories(monitor_mail
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
        )

target_link_libraries(monitor_mail
        PRIVATE appbase
        PRIVATE ${Boost_LIBRARIES} #boost test
        PRIVATE -Wl,${whole_archive_flag} log_plugin                        -Wl,${no_whole_archive_flag}
        PRIVATE -Wl,${whole_archive_flag} monitor_mail_plugin                   -Wl,${no_whole_archive_flag}
        PRIVATE -Wl,${whole_archive_flag} send_mail_plugin                      -Wl,${no_whole_archive_flag}
        )