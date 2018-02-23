cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(navilink VERSION 0.1 LANGUAGES C)

# Find packages path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(LibSerialport REQUIRED)
add_subdirectory(lib)
add_subdirectory(cli)
