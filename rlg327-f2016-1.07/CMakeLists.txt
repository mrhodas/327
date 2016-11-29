cmake_minimum_required(VERSION 3.6)
project(rlg327_f2016_1_07)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    images/base.pgm
    images/hello.pgm
    images/smile.pgm
    images/welldone.pgm
    save_files/io.h
    character.cpp
    character.h
    descriptions.cpp
    descriptions.h
    dice.cpp
    dice.h
    dims.h
    dungeon.cpp
    dungeon.h
    event.cpp
    event.h
    heap.cpp
    heap.h
    io.cpp
    io.h
    macros.h
    Makefile
    move.cpp
    move.h
    npc.cpp
    npc.h
    path.cpp
    path.h
    pc.cpp
    pc.h
    rlg327.cpp
    utils.cpp
    utils.h item.h)

add_executable(rlg327_f2016_1_07 ${SOURCE_FILES})