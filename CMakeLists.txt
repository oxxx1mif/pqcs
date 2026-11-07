cmake_minimum_required(VERSION 3.20)
project(pqcs VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PQCS_BUILD_TESTS "Build tests with Boost.Test" OFF)
option(PQCS_BUILD_BENCHMARKS "Build benchmarks with Google Benchmark" OFF)

set(CMAKE_TRY_COMPILE_CONFIGURATION "Release")

find_package(hwy QUIET)
if(NOT hwy_FOUND)
    message(STATUS "System Highway not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        highway
        GIT_REPOSITORY  https://github.com/google/highway.git
        GIT_TAG        1.2.0
        GIT_SHALLOW    TRUE
    )
    set(HWY_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(highway)
else()
    message(STATUS "Using system Highway")
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if(PQCS_BUILD_BENCHMARKS)
    find_package(benchmark QUIET)
    if(NOT benchmark_FOUND)
        message(STATUS "System Benchmark not found, fetching from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            benchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG        v1.9.0
            GIT_SHALLOW    TRUE
        )
        set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
        set(BENCHMARK_ENABLE_LTO ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(benchmark)
    else()
        message(STATUS "Using system Benchmark")
    endif()
endif()

add_library(pqcs_math INTERFACE)
target_include_directories(pqcs_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(TARGET hwy::hwy)
    target_link_libraries(pqcs_math INTERFACE hwy::hwy)
elseif(TARGET hwy)
    target_link_libraries(pqcs_math INTERFACE hwy)
else()
    target_link_libraries(pqcs_math INTERFACE -lhwy)
endif()

if(PQCS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PQCS_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

