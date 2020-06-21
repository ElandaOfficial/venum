cmake_minimum_required(VERSION 3.16)
project(Venum VERSION 1.0.0)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

if (COMMAND COMPILE_TESTS)
    add_executable(venum test/main.cpp)
endif()
