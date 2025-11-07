cmake_minimum_required(VERSION 3.25)

project(DiSH)

add_compile_options(-Wall)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRC ${PROJECT_SOURCE_DIR}/src)
set(SRCS
    ${SRC}/string_buffer.cpp
    ${SRC}/lexer.cpp
)

# add_executable(dish
#     ${SRC}/string_buffer.cpp
#     ${SRC}/lexer.cpp
#     ${SRC}/parser.cpp
#     ${SRC}/executor.cpp

#     ${SRC}/main.cpp
# )

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/unit)
endif()
