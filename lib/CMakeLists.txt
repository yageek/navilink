# At LEAST 2.8 but newer is better
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(navilink VERSION 0.1 LANGUAGES C)

if(UNIT_TEST)
	add_subdirectory(tests)
endif(UNIT_TEST)


set(SOURCES 
            src/navilink_packet.c
            src/navilink_errors.c
            src/navilink_device.c
            src/navilink_gpx.c)
set(HEADERS 
            include/navilink.h
            include/navilink_endianness.h
            include/navilink_errors.h
            include/navilink_packet.h
            include/navilink_protocol.h
            include/navilink_device.h
            include/navilink_gpx.h)

add_library(navilink STATIC ${SOURCES} ${HEADERS})

target_include_directories(navilink
    PUBLIC include
    PRIVATE src
)

include_directories(navilink ${LIBSERIALPORT_INCLUDE_DIRS})
target_link_libraries(navilink ${LIBSERIALPORT_LIBRARY})
install(TARGETS navilink DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/navilink)