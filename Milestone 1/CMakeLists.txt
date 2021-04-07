cmake_minimum_required(VERSION 3.17)
project(Milestone_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_CXX_STANDARD 14)

add_executable(Milestone_1 GUI.cpp CPU.h)