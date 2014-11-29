cmake_minimum_required(VERSION 2.8)
project(bigreader)

option(BUILD_BIG_EXTRACTOR "Builds extractor program for .big files" OFF)
option(BUILD_BIGREADER_TEST "Builds test binary for bigreader" OFF)

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp" @ONLY)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log)
endif()

# Is this the right place to link?
target_link_libraries(${PROJECT_NAME} ${Boost_FILESYSTEM_LIBRARY_RELEASE})

if(BUILD_BIGREADER_TEST)
    add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} log)
endif()

if(BUILD_BIG_EXTRACTOR)
    add_executable(big_extractor extractor/big_extractor.cpp)
    target_link_libraries(big_extractor ${PROJECT_NAME} log)
endif()
