cmake_minimum_required(VERSION 4.1)
project(SmartTrainStation)

set(CMAKE_CXX_STANDARD 20)

add_executable(SmartTrainStation main.cpp
        Train/Train.cpp
        Train/Train.h)
