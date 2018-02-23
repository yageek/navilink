# - Try to find libserialport
# Once done this will define
#  LIBSERIALPORT_FOUND - System has libserialport
#  LIBSERIALPORT_INCLUDE_DIR - The libserialport include directories
#  LIBSERIALPORT_LIBRARY - The library needed to use libserialport

find_path(LIBSERIALPORT_INCLUDE_DIR libserialport.h
          HINTS /usr/include /usr/local/include)

find_library(LIBSERIALPORT_LIBRARY NAMES libserialport serialport)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibSerialPort  DEFAULT_MSG
                                  LIBSERIALPORT_LIBRARY LIBSERIALPORT_INCLUDE_DIR)

mark_as_advanced(LIBSERIALPORT_INCLUDE_DIR LIBSERIALPORT_LIBRARY )