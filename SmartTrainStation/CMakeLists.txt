cmake_minimum_required(VERSION 4.1)
project(SmartTrainStation)

set(CMAKE_CXX_STANDARD 20)

add_executable(SmartTrainStation
        main.cpp
        Train/Train.cpp
        Train/Train.h
        Train/PassengerTrain.cpp
        Train/PassengerTrain.h
        Platform.cpp
        Platform.h
        Station/Station.cpp
        Station/Station.h
        SimulatedClock.cpp
        SimulatedClock.h
)

target_include_directories(SmartTrainStation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
