cmake_minimum_required(VERSION 2.8)
set(PROJECT_ROOT ../../)
project(test_run)

file(GLOB RAW_SRC "../../src/**/*.c")
file(GLOB TEST_SRC "*.c")

include_directories(${PROJECT_ROOT}src/alg)
include_directories(${PROJECT_ROOT}src/bug)
include_directories(${PROJECT_ROOT}src/dat)
include_directories(${PROJECT_ROOT}src/io)
include_directories(${PROJECT_ROOT}src/ipc)
include_directories(${PROJECT_ROOT}src/net)
include_directories(${PROJECT_ROOT}src/sig)
include_directories(${PROJECT_ROOT}src/sys)
include_directories(${PROJECT_ROOT}src/thr)

add_executable(test_run ${RAW_SRC} ${TEST_SRC})
target_link_libraries(test_run pthread)
