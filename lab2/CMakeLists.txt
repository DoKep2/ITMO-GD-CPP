cmake_minimum_required(VERSION 3.10)

project(lab2)

add_library(lab2_lib
        array/array.cpp
        array/array.h
        main.cpp
)

add_subdirectory(lib/googletest)

add_executable(runTests test/test.cpp)

include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(runTests lab2_lib gtest gtest_main)

enable_testing()

add_test(NAME MyTest COMMAND runTests)