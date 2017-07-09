cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(gtest-demo)

enable_language(C)
enable_language(CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/worker_pool
    ${PROJECT_SOURCE_DIR}/priority_task
    ${PROJECT_SOURCE_DIR}/priority_task_queue
    )

add_library(
    worker_pool
    worker_pool/worker_pool.h
    priority_task/priority_task.h
    priority_task_queue/priority_task_queue.h
    )

set_target_properties(worker_pool PROPERTIES LINKER_LANGUAGE CXX)

set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")

include_directories(
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    )

set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

add_executable(
    unit_tests
    tests/tests.cpp
    tests/test_constructor_destructor.cpp
    tests/test_push.cpp
    tests/test_pause_resume.cpp
    tests/test_worker_count.cpp
    tests/test_priority_task_queue.cpp
    tests/test_qsort.cpp
    tests/test_matrixmult.cpp
    tests/simulate_work.cpp
    tests/increment_parallel_count.cpp
    )

add_dependencies(unit_tests googletest)

find_package(Threads)

target_link_libraries(
    unit_tests
    googletest
    worker_pool
    ${CMAKE_THREAD_LIBS_INIT}
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)