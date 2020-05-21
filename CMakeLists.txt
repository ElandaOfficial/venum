cmake_minimum_required(VERSION 3.16)
project(venum)

set(CMAKE_CXX_STANDARD 17)

add_executable(venum test/main.cpp src/venum.h test/enummap_example.h)
