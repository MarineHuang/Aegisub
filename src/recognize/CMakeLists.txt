cmake_minimum_required(VERSION 3.5)

find_package(CURL REQUIRED)
find_package(openssl REQUIRED)

message("CURL_LIBRARIES: ${CURL_LIBRARIES}")
message("OPENSSL_LIBRARIES: ${OPENSSL_LIBRARIES}")

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog_recognize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileTrans.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NlsToken.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reg2subtitle.cpp
    ${PROJECT_SOURCES}
    CACHE FILEPATH "source files for top project" FORCE
)

set(ALI_OSS ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/aliyun-oss-cpp-sdk)

set(PROJECT_INCLUDE
    ${ALI_OSS}/sdk/include
    ${CURL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)

set(PROJECT_LIBS
    cpp-sdk
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