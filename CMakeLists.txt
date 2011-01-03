cmake_minimum_required(VERSION 2.8)
PROJECT(navilink)

SET(CMAKE_BUILD_TYPE Debug)

SET(navilink_src main.c packetapi.c NaviGPSapi.c endianness.c)
include_directories(include)


OPTION(BIG_ENDIAN OFF)
if(BIG_ENDIAN)
	add_definitions(-DBIG_ENDIAN)
endif(BIG_ENDIAN)

if(UNIX)
	MESSAGE(STATUS " == Unix plateform ==")
	SET(serial_api posixserial.c ipc_linux.c)
	add_definitions(-D_GNU_SOURCE)
endif(UNIX)
if(WIN32)
	MESSAGE(STATUS " == Win32 plateform ==")
	SET(serial_api win32serial.c)
endif(WIN32)


ADD_EXECUTABLE(navilink ${navilink_src} ${serial_api})
