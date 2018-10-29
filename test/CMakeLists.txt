add_executable(test_piglet
    test_file_io.cpp
    test_figchar.cpp
    test_figfont.cpp
    test_charsmush.cpp
    test_strsmush.cpp
    test_smusher.cpp
    test.cpp)
target_compile_definitions(test_piglet PRIVATE TOP_DIR="${PROJECT_SOURCE_DIR}")

