#cmake_minimum_required (VERSION 2.8) 

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/)

add_library(gtest gtest-all.cc)

add_library(gtest_main gtest_main.cc)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/mybuild/lib)

target_link_libraries(gtest_main gtest)

find_package(Threads)

if (CMAKE_USE_PTHREADS_INIT)
	target_link_libraries(gtest_main ${CMAKE_THREAD_LIBS_INIT})   
endif()
