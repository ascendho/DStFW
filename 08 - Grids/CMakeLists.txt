cmake_minimum_required(VERSION 4.0)
project(08___Grids LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(-Wall -Wextra -pedantic)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif ()

# Collect sources and headers
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "src/*.cpp" "src/*.cc" "src/*.cxx"
)

if(PROJECT_SOURCES)
        add_library(dstfw_core STATIC ${PROJECT_SOURCES})
        target_include_directories(dstfw_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
        target_compile_features(dstfw_core PUBLIC cxx_std_20)
endif()

# 打印配置信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
        enable_testing()
        find_package(GTest REQUIRED)

        include_directories(${CMAKE_SOURCE_DIR}/include)

        # Test: Grid (insert/delete) + Nearest Neighbor searches
        add_executable(test_grid test/test_grid.cpp)
        if(TARGET dstfw_core)
                target_link_libraries(test_grid PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_grid PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_grid PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME grid_test COMMAND test_grid)

        message(STATUS "Building with tests enabled")
endif()
