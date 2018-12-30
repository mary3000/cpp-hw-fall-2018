cmake_minimum_required(VERSION 3.13)
project(Ded)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_subdirectory(lib/google-test)

include_directories(lib/google-test/googletest/include)
include_directories(lib/google-test/googlemock/include)

add_executable(QuadraticSolverTest QuadraticSolver/src/Polynomial.cpp QuadraticSolver/src/Polynomial.h QuadraticSolver/test/Test.cpp)

add_executable(PoemSort PoemSort/main.cpp)

add_executable(ProtectedStack ProtectedStack/src/main.cpp ProtectedStack/src/stack.h)
add_executable(ProtectedStackTest ProtectedStack/test/test.cpp ProtectedStack/src/stack.h)

add_executable(Processor Processor/main.cpp Processor/src/Processor.h)
add_executable(ProcessorTest Processor/src/Processor.h Processor/test/test.cpp)

add_executable(Differentiator Differentiator/main.cpp Differentiator/src/Differentiator.h Differentiator/src/DiffNode.h Differentiator/src/DiffFunc.h)

target_link_libraries(QuadraticSolverTest gtest gtest_main)
target_link_libraries(QuadraticSolverTest gmock gmock_main)

target_link_libraries(ProtectedStackTest gtest gtest_main)
target_link_libraries(ProtectedStackTest gmock gmock_main)

target_link_libraries(ProcessorTest gtest gtest_main)
target_link_libraries(ProcessorTest gmock gmock_main)