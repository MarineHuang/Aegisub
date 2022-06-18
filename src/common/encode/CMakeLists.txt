cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/charset.cc
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

if(GTEST_ON)
    set(TEST_SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_encode.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/charset.cc
        ${TEST_SOURCES}
        CACHE STRING "source files for test" FORCE
    )
endif()
