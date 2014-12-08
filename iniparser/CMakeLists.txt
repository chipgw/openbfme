cmake_minimum_required(VERSION 2.8)
project(iniparser)

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp" @ONLY)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log bigreader)
endif()

add_test_exe(bigreader)
