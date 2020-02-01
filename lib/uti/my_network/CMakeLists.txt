cmake_minimum_required(VERSION 3.15)
project(my_network_library)

set(CMAKE_CXX_STANDARD		    20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(BINARY my_network)

if (CONAN_BOOST_ROOT)
    set(BOOST_ROOT CONAN_BOOST_ROOT)
endif(CONAN_BOOST_ROOT)

if (DEFINED ENV{BOOST_ROOT})
    set(Boost_NO_SYSTEM_PATHS TRUE)
    set(Boost_ROOT $ENV{BOOST_ROOT})
    set(Boost_INCLUDE_DIRS "${BOOST_ROOT}/include")
    set(Boost_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif()

find_package(Boost 1.71.0 COMPONENTS serialization REQUIRED)
find_package(Threads REQUIRED)

add_library(${BINARY} STATIC
        Async/ServerTcpAsyncWrapper.cpp
        Async/ClientTcpAsyncWrapper.cpp
        #BlockingMultiThread/Server/ServerUdpMultiThreadWrapper.cpp
        BlockingMultiThread/Client/ClientUdpMultiThreadWrapper.cpp
        )

target_include_directories(${BINARY} SYSTEM PUBLIC
        ${Boost_INCLUDE_DIRS}
        )

target_include_directories(${BINARY} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ./../my_string/
        Async
        #${CMAKE_CURRENT_SOURCE_DIR}/BlockingMultiThread
        #${CMAKE_CURRENT_SOURCE_DIR}/BlockingMultiThread/Server
        )

target_link_libraries(${BINARY} ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${BINARY} PRIVATE
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
elseif(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

if (MSVC)
    # Tells the compiler you are on Windows 10. It suppress warnings.
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

enable_testing()
add_subdirectory(tests/)
