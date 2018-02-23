
add_executable(navilink-cli main.c)
include_directories(navilink-cli ${LIBSERIALPORT_INCLUDE_DIRS} navilink)
target_link_libraries(navilink-cli ${LIBSERIALPORT_LIBRARY} navilink)

install(TARGETS navilink-cli DESTINATION bin)