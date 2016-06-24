cmake_minimum_required (VERSION 2.6) 

project(MpClient CXX C)

add_subdirectory(src)

add_subdirectory(unittest)
	
add_subdirectory(sample)

add_subdirectory(3rd/gtest)
	
add_subdirectory(3rd/google/protobuf)


if(WIN32)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/win32/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/win32/lib)
endif(WIN32)
