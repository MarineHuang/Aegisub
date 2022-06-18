cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/md5.impl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MD5.impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Crc32.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/Crc64.cc
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

if(GTEST_ON)
    set(TEST_SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/test_digest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/md5.impl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/MD5.impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Crc32.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/Crc64.cc 
        ${TEST_SOURCES}
        CACHE STRING "source files for test" FORCE
    )
endif()