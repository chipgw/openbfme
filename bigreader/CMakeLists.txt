cmake_minimum_required(VERSION 2.8)
project(bigreader)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} log)
endif()

if(NOT STD_FILESYSTEM)
    # Is this the right place to link?
    target_link_libraries(${PROJECT_NAME} ${Boost_FILESYSTEM_LIBRARY_RELEASE})
endif(NOT STD_FILESYSTEM)

add_test_exe()
