cmake_minimum_required(VERSION 3.20)

project(
    self_balancing_tree_benchmark
    VERSION 1.0.0
    DESCRIPTION "Benchmark comparing BST, AVL, and Splay Trees"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    tree_benchmark
    src/benchmark.cpp
)

target_include_directories(
    tree_benchmark
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(
        tree_benchmark
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

add_executable(
    tree_tests
    tests/test_trees.cpp
)

target_include_directories(
    tree_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(
        tree_tests
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_test(
    NAME tree_correctness_tests
    COMMAND tree_tests
)