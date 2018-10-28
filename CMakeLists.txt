cmake_minimum_required(VERSION 2.8.9)
project(piglet)
add_subdirectory(src)
add_subdirectory (test)
enable_testing()
add_test(NAME full COMMAND test_piglet)

