add_executable( hb_test hb_test.cpp )

target_link_libraries( hb_test hb ${Boost_LIBRARIES} pthread)

add_test(NAME hb_test COMMAND hb_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR})