cmake_minimum_required(VERSION 3.17)
project(Unit_Testing)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Unit_Testing TestDriver.cpp test.h VIEW.h CONTROLLER.h MEMORY.h MODEL.h CPU.h)

#find_package(Threads)
#target_link_libraries(${PROJECT_NAME} Threads::Threads)
