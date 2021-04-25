cmake_minimum_required(VERSION 3.17)
project(Milestone_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_STANDARD 17)

#include_directories(.)
#find_path(Unit\ Testing/ [CMAKE_IGNORE_PATH])

add_executable(Milestone_3
        main.cpp
        CONTROLLER.h
        MODEL.h
        MEMORY.h
        CPU.h
        VIEW.h
        MenuStrings.h
        MemoryDump.h)
