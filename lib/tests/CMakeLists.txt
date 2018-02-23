# Look for CUNIT.
find_package(CUnit REQUIRED)

add_library(cunit INTERFACE IMPORTED)
set_property(TARGET cunit PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${CUNIT_INCLUDE_DIR})
set_property(TARGET cunit PROPERTY
    INTERFACE_LINK_LIBRARIES ${CUNIT_LIBRARY})

# Create unit test executable.
add_executable(test_navilink test_navilink.c)
target_link_libraries(test_navilink cunit navilink)