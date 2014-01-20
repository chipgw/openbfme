cmake_minimum_required(VERSION 2.8)
project(bigreader)

option(BIG_EXTRACTOR "Builds extractor program for .big files" OFF)
option(BIGREADER_TEST "Builds test binary for bigreader" OFF)

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp" @ONLY)

# files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.h")

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log)
endif()

if(BIGREADER_TEST)
    add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} log)
endif()

if(BIG_EXTRACTOR)
    add_executable(big_extractor extractor/big_extractor.cpp)
    target_link_libraries(big_extractor ${PROJECT_NAME} log)
endif()
