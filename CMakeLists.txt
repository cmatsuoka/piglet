cmake_minimum_required(VERSION 2.8.9)
project(piglet)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory (test)
    add_test(NAME full COMMAND test_piglet)
endif()

