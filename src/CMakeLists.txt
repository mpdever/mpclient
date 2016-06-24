#cmake_minimum_required (VERSION 2.8) 

aux_source_directory(. DIR_SRCS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/ 
		/usr/local/ssl/include/)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/mybuild/lib)

add_library(mpclient ${DIR_SRCS})

if(WIN32)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/win32/include)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/win32/lib)
#	find_library(SSLEAY ssleay32 ${PROJECT_SOURCE_DIR}/win32/lib)
#	find_library(LIBEAY libeay32 ${PROJECT_SOURCE_DIR}/win32/lib)
#	find_library(LIBCURL libcurl ${PROJECT_SOURCE_DIR}/win32/lib)
#	target_link_libraries(mpclient ${SSLEAY} ${LIBEAY} ${LIBCURL})
	target_link_libraries(mpclient protobuf libeay32 ssleay32 libcurl)
else(WIN32)
	LINK_DIRECTORIES(/usr/local/lib/ /usr/local/ssl/lib/)
	target_link_libraries(mpclient protobuf ssl crypto curl)
endif(WIN32)
