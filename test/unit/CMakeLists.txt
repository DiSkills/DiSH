cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

set(TESTS
    lexer.cpp
)

foreach(test ${TESTS})
    get_filename_component(name ${test} NAME_WE)
    set(name "test_${name}")

    add_executable(${name} ${test} ${SRCS})
    target_link_libraries(${name} GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${name})
endforeach()
