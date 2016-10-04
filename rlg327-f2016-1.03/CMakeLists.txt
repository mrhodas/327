cmake_minimum_required(VERSION 3.6)
project(rlg327_f2016_1_03)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    images/base.pgm
    images/hello.pgm
    images/smile.pgm
    images/welldone.pgm
    save_files/0.rlg327
    save_files/1.rlg327
    save_files/2.rlg327
    save_files/3.rlg327
    save_files/4.rlg327
    save_files/5.rlg327
    save_files/6.rlg327
    save_files/7.rlg327
    save_files/8.rlg327
    save_files/9.rlg327
    save_files/hello.rlg327
    save_files/smile.rlg327
    save_files/welldone.rlg327
    dims.h
    dungeon.c
    dungeon.h
    heap.c
    heap.h
    macros.h
    path.c
    path.h
    rlg327.c
    utils.c
    utils.h)

add_executable(rlg327_f2016_1_03 ${SOURCE_FILES})