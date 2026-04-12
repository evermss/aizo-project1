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
        src/QuickSorter.cpp
        src/ShellSorter.cpp
        src/BucketSorter.cpp
        src/SortChecker.cpp
        src/CsvReportWriter.cpp
        src/BinarySearchTree.cpp
        src/Stack.cpp
        src/research/ResearchRunner.cpp

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