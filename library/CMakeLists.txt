cmake_minimum_required(VERSION 3.0.2)
project(ExploringRPi)
find_package (Threads)
set(CMAKE_BUILD_TYPE Release)

# Only available from version 2.8.9 on
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Bring the headers, such as BusDevice.h into the project
include_directories(bus display gpio motor network sensor)

# The file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "./*.cpp")

# Can build statically to ExploringRPi.a using the next line
#add_library(ExploringRPi STATIC ${SOURCES})

# Building shared library to ExploringRPi.so using the next line
add_library(ExploringRPi SHARED ${SOURCES})

target_link_libraries(ExploringRPi ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS ExploringRPi DESTINATION /usr/lib)
