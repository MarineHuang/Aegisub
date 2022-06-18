cmake_minimum_required(VERSION 3.5)

#include(FindCURL)
#include(FindOpenSSL)
#
#if(NOT CURL_FOUND)
#    message(FATAL_ERROR "Could not find curl")
#endif()
#message(${CURL_LIBRARIES})
#
#if(NOT OPENSSL_FOUND)
#    message(FATAL_ERROR "Could not find openssl")
#endif()
#message(${OPENSSL_LIBRARIES})
set(CURL_INCLUDE_DIRS 
    "D:/tmpGitee/aegi-deps/curl-7.81.0/builds/libcurl-vc14-x64-release-dll-ipv6-sspi-schannel/include"
) 
set(CURL_LIBRARIES 
    "D:/tmpGitee/aegi-deps/curl-7.81.0/builds/libcurl-vc14-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib"
)

#set(OPENSSL_INCLUDE_DIR "D:/tmpGitee/openssl/include") 
#set(OPENSSL_LIBRARIES 
#    "D:/tmpGitee/openssl/lib/libssl.lib"
#    "D:/tmpGitee/openssl/lib/libcrypto.lib"
#)

file(GLOB NETWORK_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(PROJECT_SOURCES
    ${NETWORK_SOURCES}
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

set(PROJECT_INCLUDE
    ${OPENSSL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)

set(PROJECT_LIBS
    #${OPENSSL_LIBRARIES}
    ${CURL_LIBRARIES}
    #Rpcrt4.lib 
    Crypt32.lib
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