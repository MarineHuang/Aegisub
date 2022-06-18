cmake_minimum_required(VERSION 3.5)


set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/aligner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/_levenshtein.c
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

#set(PROJECT_INCLUDE
#    
#    ${PROJECT_INCLUDE}
#    CACHE PATH "include directory for top project" FORCE
#)

#set(PROJECT_LIBS
#    
#    ${PROJECT_LIBS}
#    CACHE STRING "libraries for top project" FORCE
#)

if(GTEST_ON)
    #set(TEST_SOURCES 
    #    ${CMAKE_CURRENT_SOURCE_DIR}/uuid.cpp 
    #    ${TEST_SOURCES}
    #    CACHE STRING "source files for test" FORCE
    #)
endif()