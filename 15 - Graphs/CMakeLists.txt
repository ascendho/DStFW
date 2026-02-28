cmake_minimum_required(VERSION 4.0)
project(15___Graphs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -pedantic)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    include_directories(${CMAKE_SOURCE_DIR}/include)

    add_executable(test_dijkstra test/test_dijkstra.cpp)
    target_link_libraries(test_dijkstra PRIVATE GTest::gtest_main)
    set_target_properties(test_dijkstra PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    add_test(NAME test_dijkstra COMMAND test_dijkstra)

    add_executable(test_prim test/test_prim.cpp)
    target_link_libraries(test_prim PRIVATE GTest::gtest_main)
    set_target_properties(test_prim PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    add_test(NAME test_prim COMMAND test_prim)

    add_executable(test_kahn test/test_kahn.cpp)
    target_link_libraries(test_kahn PRIVATE GTest::gtest_main)
    set_target_properties(test_kahn PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    add_test(NAME test_kahn COMMAND test_kahn)
endif()
