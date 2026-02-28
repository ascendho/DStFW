cmake_minimum_required(VERSION 4.0)
project(03___Dynamic_Data_Structures LANGUAGES CXX)

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

        # Test: Array Doubling
        add_executable(test_array_doubling
                test/test_array_doubling.cpp
        )
        if(TARGET dstfw_core)
                target_link_libraries(test_array_doubling PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_array_doubling PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_array_doubling PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME array_doubling_test COMMAND test_array_doubling)

        # Test: Linked List
        add_executable(test_linked_list
                test/test_linked_list.cpp
        )
        if(TARGET dstfw_core)
                target_link_libraries(test_linked_list PRIVATE dstfw_core GTest::gtest_main)
        else()
                target_link_libraries(test_linked_list PRIVATE GTest::gtest_main)
        endif()
        set_target_properties(test_linked_list PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_test(NAME linked_list_test COMMAND test_linked_list)

        message(STATUS "Building with tests enabled")
endif()
