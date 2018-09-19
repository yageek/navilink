add_executable(navilink-cli main.c)
include_directories(navilink-cli ${LIBSERIALPORT_INCLUDE_DIR} navilink)
target_link_libraries(navilink-cli ${LIBSERIALPORT_LIBRARY} navilink)
if(APPLE)
    FIND_LIBRARY(Foundation Foundation)
    find_library(IOKit IOKit)
    target_link_libraries(navilink-cli ${Foundation} ${IOKit})
endif()


install(TARGETS navilink-cli DESTINATION bin)