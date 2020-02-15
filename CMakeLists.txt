cmake_minimum_required(VERSION 3.14.5)
project(My_Apache)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

if (MSVC)
    # Tells the compiler you are on Windows 10. It suppress warnings.
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

add_subdirectory("${CMAKE_HOME_DIRECTORY}/lib/libmy_parsers/")
add_subdirectory("${CMAKE_HOME_DIRECTORY}/lib/uti/my_network/")
add_subdirectory("${CMAKE_HOME_DIRECTORY}/lib/openZia")
add_subdirectory("${CMAKE_HOME_DIRECTORY}/srcs/")
