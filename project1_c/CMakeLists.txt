cmake_minimum_required(VERSION 3.6)
project(project1_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c uthread.h uthread.c uthread_map.c uthread_map.h thread.c thread.h uthread_queue.c uthread_queue.h)
add_executable(project1_c ${SOURCE_FILES})