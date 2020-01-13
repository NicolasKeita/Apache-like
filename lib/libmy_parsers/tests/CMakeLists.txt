cmake_minimum_required(VERSION 3.14.5)
project(libmy_parsers)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_executable(tests_libmy_parsers Test_ParserJsonWrapper.cpp)
target_link_libraries(tests_libmy_parsers my_parsers)

target_include_directories(tests_libmy_parsers PUBLIC ../)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(tests_libmy_parsers PRIVATE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast)
endif(CMAKE_COMPILER_IS_GNUCXX)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/json_correct.json
        json_correct.json
        COPYONLY)

add_test(NAME test_name COMMAND tests_libmy_parsers)
