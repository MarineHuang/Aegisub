cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cc
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

if(GTEST_ON)
    set(TEST_SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_string.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cc 
        ${TEST_SOURCES}
        CACHE STRING "source files for test" FORCE
    )
endif()
