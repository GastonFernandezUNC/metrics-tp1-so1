cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    "memory_fragmentation"
    VERSION 1.0
    LANGUAGES C
)

include_directories(include)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g -pthread")

add_executable(${PROJECT_NAME} src/main.c src/functions.c)