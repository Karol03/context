cmake_minimum_required(VERSION 3.5)

project(context LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

add_executable(context
    main.cpp

    ctx/context.hpp
    ctx/literal.hpp
    ctx/typeof.hpp
    ctx/valuestorage.hpp
)
