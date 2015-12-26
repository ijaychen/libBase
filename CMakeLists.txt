cmake_minimum_required(VERSION 2.4)
include("CMakeVar.txt")

aux_source_directory(. DIR_SRCS)
file(GLOB sources1 "*.cpp" "*/*.cpp" "*/*/*.cpp" "*/*/*/*.cpp")

add_executable(test
	${sources1}
)

