cmake_minimum_required(VERSION 3.6)
project(1_03_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    save_files/._0.rlg327
    save_files/._1.rlg327
    save_files/._2.rlg327
    save_files/._3.rlg327
    save_files/._4.rlg327
    save_files/._5.rlg327
    save_files/._6.rlg327
    save_files/._7.rlg327
    save_files/._8.rlg327
    save_files/._9.rlg327
    save_files/._hello.rlg327
    save_files/._smile.rlg327
    save_files/._welldone.rlg327
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
    ._corridor.c
    ._save_files
    character.c
    character.h
    core.h
    corridor.c
    corridor.h
    debug.c
    debug.h
    dijkstra.c
    dijkstra.h
    dungeon.c
    dungeon.h
    heap.c
    heap.h
    main.c
    point.c
    point.h
    room.c
    room.h)

add_executable(1_03_2 ${SOURCE_FILES})