cmake_minimum_required(VERSION 3.29)
project(trace)

set(CMAKE_CXX_STANDARD 20)

add_executable(trace
        main.cpp
        src/transaction.cpp
        include/transaction.h)
