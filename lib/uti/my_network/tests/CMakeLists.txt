cmake_minimum_required(VERSION 3.12.4)
project(my_network_library_tests)

if(NOT CMAKE_COMPILER_IS_GNUCXX)
    return()
endif(NOT CMAKE_COMPILER_IS_GNUCXX)

find_package(Boost 1.69.0 REQUIRED)
find_package(Threads)
find_program(BASH_PROGRAM bash)

if (MSVC)
    include_directories(SYSTEM
            ${BOOST_INCLUDE_DIRS}
            )
else()
    include_directories(SYSTEM
            ${Boost_INCLUDE_DIRS}
            )
endif()

include_directories(
        ./
        ../BlockingMultiThread
        ../Async)

add_executable(test_serv test_1_source_code/serv.cpp)
add_executable(test_client test_1_source_code/client.cpp)
target_link_libraries(test_serv
        ${CMAKE_THREAD_LIBS_INIT}
        my_network)
target_link_libraries(test_client
        ${CMAKE_THREAD_LIBS_INIT}
        my_network)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_10_msgs.sh test_10_msgs.sh COPYONLY)
add_test(test_network_0
        ${CMAKE_COMMAND} --build ./)
add_test(test_network_1
        ${BASH_PROGRAM} test_10_msgs.sh)
set_tests_properties(test_network_1 PROPERTIES DEPENDS test_network_0)