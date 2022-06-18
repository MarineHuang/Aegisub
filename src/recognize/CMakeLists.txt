cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog_recognize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileTrans.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NlsToken.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reg2subtitle.cpp
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

set(CURL_INCLUDE_DIR
    "D:/tmpGitee/aegi-deps/curl-7.81.0/builds/libcurl-vc14-x64-release-dll-ipv6-sspi-schannel/include"
) 
set(CURL_LIBRARIES 
    "D:/tmpGitee/aegi-deps/curl-7.81.0/builds/libcurl-vc14-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib"
)

set(OPENSSL_INCLUDE_DIR "D:/tmpGitee/openssl/include") 
set(OPENSSL_LIBRARIES 
    "D:/tmpGitee/openssl/lib/libssl.lib"
    "D:/tmpGitee/openssl/lib/libcrypto.lib"
)

set(ALI_OSS D:\\tmpGitee\\aliyun\\oss_win\\sdk)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(ALI_OSS_LIB ${ALI_OSS}/lib_debug/alibabacloud-oss-cpp-sdk.lib)
else()
    set(ALI_OSS_LIB ${ALI_OSS}/lib/alibabacloud-oss-cpp-sdk.lib)
endif()

set(PROJECT_INCLUDE
    ${ALI_OSS}/include
    ${CURL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)

set(PROJECT_LIBS
    ${ALI_OSS_LIB}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
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