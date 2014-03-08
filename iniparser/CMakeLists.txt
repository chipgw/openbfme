cmake_minimum_required(VERSION 2.8)
project(iniparser)

option(INIPARSER_TEST "Builds test binary for iniparser" OFF)

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp" @ONLY)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log bigreader)
endif()

if(INIPARSER_TEST)
    add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} log bigreader)
endif()
