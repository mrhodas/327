cmake_minimum_required(VERSION 3.6)
project(rlg327_f2016_1_09)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    data_files/monster_desc.txt
    data_files/object_desc.txt
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
    save_files/io.h
    save_files/smile.rlg327
    save_files/welldone.rlg327
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
    main.cpp
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

add_executable(rlg327_f2016_1_09 ${SOURCE_FILES})