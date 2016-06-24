#cmake_minimum_required (VERSION 2.8) 

aux_source_directory(. DIR_SRCS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/mybuild/unittest)

add_executable(test_basic unittest_basic.cpp)
target_link_libraries(test_basic gtest_main mpclient)

add_executable(test_menu unittest_menu.cpp)
target_link_libraries(test_menu gtest_main mpclient)

add_executable(test_media unittest_media.cpp)
target_link_libraries(test_media gtest_main mpclient)

add_executable(test_message unittest_message.cpp)
target_link_libraries(test_message gtest_main mpclient)
	
add_executable(test_template unittest_template.cpp)
target_link_libraries(test_template gtest_main mpclient)

add_executable(test_user unittest_user.cpp)
target_link_libraries(test_user gtest_main mpclient)
