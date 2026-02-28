cmake_minimum_required(VERSION 4.0)
project(09___Spatial_Trees LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(-Wall -Wextra -pedantic)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif ()

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "src/*.cpp" "src/*.cc" "src/*.cxx"
)

if(PROJECT_SOURCES)
        add_library(dstfw_core STATIC ${PROJECT_SOURCES})
        target_include_directories(dstfw_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
        target_compile_features(dstfw_core PUBLIC cxx_std_20)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
        enable_testing()
        find_package(GTest REQUIRED)

        include_directories(${CMAKE_SOURCE_DIR}/include)

        # Test: QuadTree
        add_executable(test_quadtree test/test_quadtree.cpp)
        if(TARGET dstfw_core)
                target_link_libraries(test_quadtree PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_quadtree PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_quadtree PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME quadtree_test COMMAND test_quadtree)

        # Test: KDTree
        add_executable(test_kdtree test/test_kdtree.cpp)
        if(TARGET dstfw_core)
                target_link_libraries(test_kdtree PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_kdtree PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_kdtree PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME kdtree_test COMMAND test_kdtree)

        message(STATUS "Building with tests enabled")
endif()
