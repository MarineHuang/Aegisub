cmake_minimum_required(VERSION 3.5)


set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/srt_file.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_ass.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_ebu3264.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_encore.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_microdvd.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_mkv.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_srt.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_ssa.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_transtation.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_ttxt.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/subtitle_format_txt.cpp
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