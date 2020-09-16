add_executable( fc_test fc_test.cpp )

target_link_libraries( fc_test fc ${Boost_LIBRARIES} pthread)

add_test(NAME fc_test COMMAND fc_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR})