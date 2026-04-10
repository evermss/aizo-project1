cmake_minimum_required(VERSION 3.16)

project(aizo-project1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aizo-project1
        src/main.cpp
        src/FileLoader.cpp
        src/DynamicArray.cpp
        src/SinglyLinkedList.cpp
        src/DoublyLinkedList.cpp
        src/QuickSort.cpp
        src/SortChecker.cpp
        src/QuickSort.cpp
        src/QuickSort.h
)

target_include_directories(aizo-project1 PRIVATE src)

target_compile_options(aizo-project1 PRIVATE
        -Wall
        -Wextra
        -Werror
)