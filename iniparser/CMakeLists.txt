cmake_minimum_required(VERSION 2.8)
project(iniparser)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log bigreader)
endif()

add_test_exe(bigreader)
