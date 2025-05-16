cmake_minimum_required(VERSION 3.31)
project(SOI_FIleSystem C)
set(CMAKE_C_STANDARD 11)

file(GLOB sourceFiles src/*.c)

add_executable(SOI_FIleSystem main.c ${sourceFiles})
