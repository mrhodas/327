cmake_minimum_required(VERSION 3.6)
project(1_09)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
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
    object.cpp
    object.h
    path.cpp
    path.h
    pc.cpp
    pc.h
    rlg327.cpp
    utils.cpp
    utils.h)

add_executable(1_09 ${SOURCE_FILES})