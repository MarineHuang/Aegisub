cmake_minimum_required(VERSION 3.5)

#message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/digest")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/encode")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/json")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/network")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/string")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/uuid")

set(PROJECT_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_INCLUDE}
    CACHE PATH "include directory for top project" FORCE
)