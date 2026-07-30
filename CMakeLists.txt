cmake_minimum_required(VERSION 3.14)
project(exchange_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Google Benchmark
FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/refs/heads/main.zip
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest benchmark)

include_directories(include)

# Main program
add_executable(main src/main.cpp)

# Tests
enable_testing()
add_executable(order_book_test tests/order_book_test.cpp)
target_link_libraries(order_book_test GTest::gtest_main)

# Benchmarks
add_executable(order_book_bench benchmarks/order_book_bench.cpp)
target_link_libraries(order_book_bench benchmark::benchmark_main)
