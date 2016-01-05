cmake_minimum_required(VERSION 2.4)
include("CMakeVar.txt")

set(CMAKE_BUILD_TYPE Debug)
aux_source_directory(. DIR_SRCS)
file(GLOB sources1 "*.cpp" "*/*.cpp" "*/*/*.cpp" "*/*/*/*.cpp")

add_library(base SHARED
	${sources1}
)

