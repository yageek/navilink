cmake_minimum_required(VERSION 2.8)
PROJECT(navilink)

SET(CMAKE_BUILD_TYPE Debug)

SET(navilink_src main.c api.c endianness.c)
include_directories(include)

OPTION(BIG_ENDIAN OFF)
if(BIG_ENDIAN)
	add_definitions(-DBIG_ENDIAN)
endif(BIG_ENDIAN)



ADD_EXECUTABLE(navilink ${navilink_src})

