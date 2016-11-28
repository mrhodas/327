cmake_minimum_required(VERSION 3.6)
project(rlg327_f2016_1_05)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        save_files/io.h
        Character.cpp
        Character.h
        dims.h
        dungeon.c
        dungeon.h
        event.c
        event.h
        heap.c
        heap.h
        io.c
        io.h
        macros.h
        Makefile
        move.c
        move.h
        Npc.cpp
        Npc.h
        path.c
        path.h
        Pc.cpp
        Pc.h
        rlg327.c
        utils.c
        utils.h)

add_executable(rlg327_f2016_1_05 ${SOURCE_FILES})