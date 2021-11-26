cmake_minimum_required(VERSION 3.20)
project(Ymir C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Ymir
        huffman.c
        huffman.h
        patterns.c
        patterns.h)
