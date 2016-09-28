cmake_minimum_required(VERSION 3.6)
project(1_dungeon_generation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    dungeon_generator.c
    main.h)

add_executable(1_dungeon_generation ${SOURCE_FILES})