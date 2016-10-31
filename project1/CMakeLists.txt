cmake_minimum_required(VERSION 3.6)
project(project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp uthread.h uthread.cpp Thread.cpp Thread.h)
add_executable(project1 ${SOURCE_FILES})