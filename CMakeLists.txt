cmake_minimum_required(VERSION 3.14.5)
project(My_Apache)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_subdirectory("${CMAKE_HOME_DIRECTORY}/lib/libmy_parsers/")
add_subdirectory("${CMAKE_HOME_DIRECTORY}/srcs/")