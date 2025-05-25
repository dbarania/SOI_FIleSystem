cmake_minimum_required(VERSION 3.29)
project(vfs C)
set(CMAKE_C_STANDARD 11)

file(GLOB sourceFiles src/*.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(vfs main.c ${sourceFiles})

target_include_directories(vfs PRIVATE include)
