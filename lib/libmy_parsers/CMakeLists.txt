cmake_minimum_required(VERSION 3.14.5)
project(libmy_parsers)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_library(my_parsers STATIC
        ParserJsonWrapper.cpp)

target_include_directories(my_parsers SYSTEM PUBLIC
        nolhmann_json_library/include
        ${Boost_INCLUDE_DIRS})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(my_parsers PRIVATE
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