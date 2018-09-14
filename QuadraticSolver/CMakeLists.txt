cmake_minimum_required(VERSION 3.12)
project(QuadraticSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_subdirectory(lib/google-test)

include_directories(lib/google-test/googletest/include)
include_directories(lib/google-test/googlemock/include)

add_executable(QuadraticSolver src/Polynomial.cpp src/Polynomial.h test/Test.cpp)

target_link_libraries(QuadraticSolver gtest gtest_main)
target_link_libraries(QuadraticSolver gmock gmock_main)
