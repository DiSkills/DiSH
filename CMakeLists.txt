cmake_minimum_required(VERSION 3.25)

project(DiSH)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(SRC ${CMAKE_SOURCE_DIR}/src)

add_executable(dish
    ${SRC}/string_buffer.cpp
    ${SRC}/lexer.cpp
    ${SRC}/parser.cpp
    ${SRC}/executor.cpp

    ${SRC}/main.cpp
)

target_include_directories(dish PRIVATE ${CMAKE_SOURCE_DIR})
