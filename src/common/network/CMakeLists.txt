cmake_minimum_required(VERSION 3.5)

find_package(curl REQUIRED)
find_package(openssl REQUIRED)

file(GLOB NETWORK_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(PROJECT_SOURCES
    ${NETWORK_SOURCES}
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

set(PROJECT_INCLUDE
    ${CURL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)

set(PROJECT_LIBS
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${PROJECT_LIBS}
    CACHE STRING "libraries for top project" FORCE
)

if(GTEST_ON)
    #set(TEST_SOURCES 
    #    ${CMAKE_CURRENT_SOURCE_DIR}/***.cpp 
    #    ${TEST_SOURCES}
    #    CACHE STRING "source files for test" FORCE
    #)
endif()