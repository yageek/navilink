cmake_minimum_required(VERSION 2.8)
PROJECT(navilink)

SET(navilink_src main.c api.c)

include_directories(include)

ADD_EXECUTABLE(navilink ${navilink_src})

