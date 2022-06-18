cmake_minimum_required(VERSION 3.5)

find_package(FFMPEG REQUIRED)
message("FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS}")

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_op.cpp
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

set(PROJECT_INCLUDE
    ${FFMPEG_INCLUDE_DIRS}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)

set(PROJECT_LIBS
    ${FFMPEG_LIBRARIES}
    ${PROJECT_LIBS}
    CACHE STRING "libraries for top project" FORCE
)

if(GTEST_ON)
    #set(TEST_SOURCES 
    #    ${CMAKE_CURRENT_SOURCE_DIR}/uuid.cpp 
    #    ${TEST_SOURCES}
    #    CACHE STRING "source files for test" FORCE
    #)
endif()