cmake_minimum_required(VERSION 3.29)
project(CropOptima)

set(CMAKE_CXX_STANDARD 14)

include_directories(CropOptimaProject)

add_executable(CropOptima
        CropOptimaProject/Graph.h
        CropOptimaProject/main.cpp
        CropOptimaProject/Utility.h)
