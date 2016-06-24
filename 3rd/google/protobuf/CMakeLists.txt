#cmake_minimum_required (VERSION 2.8) 

aux_source_directory(. DIR_SRCS)

aux_source_directory(io DIR_SRCS)

aux_source_directory(stubs DIR_SRCS)

aux_source_directory(testing DIR_SRCS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rd/ 
		${PROJECT_SOURCE_DIR}/3rd/google/protobuf)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/mybuild/lib)

add_library(protobuf ${DIR_SRCS})
