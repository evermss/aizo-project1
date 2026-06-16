cmake_minimum_required(VERSION 3.16)

project(aizo-project1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aizo-project1
        src/main.cpp
        src/structures/DynamicArray.cpp
        src/structures/SinglyLinkedList.cpp
        src/structures/DoublyLinkedList.cpp
        src/algorithms/QuickSorter.cpp
        src/algorithms/ShellSorter.cpp
        src/algorithms/BucketSorter.cpp
        src/algorithms/SortChecker.cpp
        src/CsvReportWriter.cpp
        src/structures/BinarySearchTree.cpp
        src/structures/Stack.cpp
        src/research/ResearchRunner.cpp
        src/research/TypeResearch.cpp
        src/FileLoader.cpp
)

target_include_directories(aizo-project1 PRIVATE
        src
        src/include
)

target_link_directories(aizo-project1 PRIVATE
        src/lib
)

target_link_libraries(aizo-project1 PRIVATE
        parameters
)

target_compile_options(aizo-project1 PRIVATE
        -Wall
        -Wextra
        -Werror
)

set_target_properties(aizo-project1 PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/src/lib"
)