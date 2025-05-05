cmake_minimum_required(VERSION 3.29)
project(untitled)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c
        common.h
        function.c)
