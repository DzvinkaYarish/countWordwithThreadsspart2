cmake_minimum_required(VERSION 3.6)
project(countWordspart2)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
set(SOURCE_FILES main.cpp workingFunctions.cpp workingFunctions.h)
add_executable(countWordspart2 ${SOURCE_FILES})