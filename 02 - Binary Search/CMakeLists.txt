cmake_minimum_required(VERSION 4.0)
project(02___Binary_Search LANGUAGES CXX)

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
file(GLOB_RECURSE PROJECT_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}
        "include/*.hpp" "include/*.tpp" "include/*.h"
)
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "src/*.cpp" "src/*.cc" "src/*.cxx"
)

# If there are any .cpp sources in src/, build them into a library
if(PROJECT_SOURCES)
        add_library(dstfw_core STATIC ${PROJECT_SOURCES})
        target_include_directories(dstfw_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
        target_compile_features(dstfw_core PUBLIC cxx_std_20)
endif()

# 打印配置信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

# 可选：添加简单的测试（默认开启）
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
        enable_testing()
        find_package(GTest REQUIRED)

        # 全局包含目录
        include_directories(${CMAKE_SOURCE_DIR}/include)

        # Test: Linear Scan
        add_executable(test_linear_scan
                test/test_linear_scan.cpp
        )
        if(TARGET dstfw_core)
                target_link_libraries(test_linear_scan PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_linear_scan PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_linear_scan PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME linear_scan_test COMMAND test_linear_scan)

        # Test: Binary Search
        add_executable(test_binary_search
                test/test_binary_search.cpp
        )
        if(TARGET dstfw_core)
                target_link_libraries(test_binary_search PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_binary_search PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_binary_search PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME binary_search_test COMMAND test_binary_search)

        message(STATUS "Building with tests enabled")
endif()
