cmake_minimum_required(VERSION 3.16)

project(SmartLibrary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SmartLibrary
    main.cpp
    books/Book.cpp
    books/Book.h
    books/PrintedBook.cpp
    books/PrintedBook.h
    library/Library.cpp
    library/Library.h
    search/Search.cpp
    search/Search.h
    users/User.cpp
    users/User.h
)

target_include_directories(SmartLibrary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
