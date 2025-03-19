cmake_minimum_required(VERSION 3.29)
project(20242_P2_DSLab C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(20242_P2_DSLab
        src/csv.c
        src/main.c
        src/subscription.c
        src/test_suite.c)
