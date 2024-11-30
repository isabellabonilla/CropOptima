cmake_minimum_required(VERSION 3.27)
project(CropOptima)

set(CMAKE_CXX_STANDARD 14)

include_directories(CropOptimaProject)

add_executable(CropOptima
        CropOptimaProject/Graph.h
        CropOptimaProject/main.cpp
        CropOptimaProject/Utility.h)

add_executable(CropDataGen
        CropOptimaProject/dataset/cropDataGenerator.cpp
)
