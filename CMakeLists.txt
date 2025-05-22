cmake_minimum_required(VERSION 3.31)
project(SOI_FileSystem C)
set(CMAKE_C_STANDARD 11)

file(GLOB sourceFiles src/*.c)

add_executable(SOI_FileSystem main.c ${sourceFiles})

target_include_directories(SOI_FileSystem PRIVATE include)
