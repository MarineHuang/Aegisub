cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/uuid.cpp
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

if(GTEST_ON)
    #set(TEST_SOURCES 
    #    ${CMAKE_CURRENT_SOURCE_DIR}/uuid.cpp 
    #    ${TEST_SOURCES}
    #    CACHE STRING "source files for test" FORCE
    #)
endif()