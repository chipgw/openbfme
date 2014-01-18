project (openbfme)
cmake_minimum_required(VERSION 2.8)

option(BUILD_CLIENT "Build the client binary." OFF)
option(BUILD_SERVER "Build the server binary." OFF)

# TODO - make this use the new detect cxx features thing when it's released.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("./bigreader/include")
include_directories("./iniparser/include")
include_directories("./log/include")

add_subdirectory(log)
add_subdirectory(bigreader)
add_subdirectory(iniparser)

if(BUILD_SERVER)
    add_subdirectory(server)
endif(BUILD_SERVER)

if(BUILD_CLIENT)
    add_subdirectory(client)
endif(BUILD_CLIENT)
