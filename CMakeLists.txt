cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    "metrics"
    VERSION 1.0
    LANGUAGES C
)

include(CTest)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g -pthread")

include_directories(include)
include_directories(include/cjson)

link_directories(/usr/local/lib)

find_package(cJSON REQUIRED)
find_package(unity REQUIRED)

add_executable(${PROJECT_NAME} src/main.c src/expose_metrics.c src/metrics.c src/cjson_handler_tp1.c)

target_link_libraries(${PROJECT_NAME} PRIVATE cjson::cjson)
target_link_libraries(${PROJECT_NAME} PRIVATE unity::unity)
target_link_libraries(${PROJECT_NAME} PRIVATE prom)
target_link_libraries(${PROJECT_NAME} PRIVATE promhttp)