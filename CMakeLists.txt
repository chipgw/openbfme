project (openbfme)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

option(BUILD_CLIENT "Build the client binary." OFF)
option(BUILD_SERVER "Build the server binary." OFF)

# TODO - make this use the new detect cxx features thing when it's released.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.
include_directories("${CMAKE_SOURCE_DIR}/bigreader/include")
include_directories("${CMAKE_SOURCE_DIR}/iniparser/include")
include_directories("${CMAKE_SOURCE_DIR}/log/include")

# files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.hpp")
add_custom_target(Headers SOURCES ${HEADERS})

add_subdirectory(log)
add_subdirectory(bigreader)
add_subdirectory(iniparser)

if(BUILD_SERVER)
    add_subdirectory(server)
endif(BUILD_SERVER)

if(BUILD_CLIENT)
    add_subdirectory(client)
endif(BUILD_CLIENT)
