cmake_minimum_required(VERSION 3.5)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/recognize")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/common")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/stream")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/aligner")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/subtitle")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/network")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/string")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/uuid")

#set(PROJECT_INCLUDE
#    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${PROJECT_INCLUDE}
#    CACHE PATH "include directory for top project" FORCE
#)