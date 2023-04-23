cmake_minimum_required(VERSION 3.22.1)

project(mazes CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
  set(CMAKE_MAKE_PROGRAM "${PROJECT_SOURCE_DIR}/scripts/make-parallel.sh" CACHE STRING "" FORCE)
endif()

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Maze must be built outside its source directory, e.g. `cmake -B build`.")
endif()

find_package( Threads REQUIRED )

include(etc/build_type.cmake)
include(etc/cflags.cmake)
include(etc/scanners.cmake)

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/builders")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/builders")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)