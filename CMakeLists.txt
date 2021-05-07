cmake_minimum_required(VERSION 3.16)
project(Venum VERSION 1.0.0)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(venum INTERFACE)
target_include_directories(venum
    INTERFACE
        include/)

if (COMPILE_TESTS)
    add_executable(venum test/main.cpp)
endif()
