cmake_minimum_required(VERSION 3.17)
project(Unit_Testing)

set(CMAKE_CXX_STANDARD 17)

add_executable(Unit_Testing TestDriver.cpp, test.h, VIEW.h)