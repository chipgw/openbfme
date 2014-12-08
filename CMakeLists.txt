project(openbfme)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

option(BUILD_CLIENT "Build the client binary." OFF)
option(BUILD_SERVER "Build the server binary." OFF)

option(STD_FILESYSTEM "Use system provided filesystem library instead of boost filesystem. Only works with MSVC currently" OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # add any future MSVC related stuff here.
endif()

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.
include_directories("${CMAKE_SOURCE_DIR}/bigreader/include")
include_directories("${CMAKE_SOURCE_DIR}/iniparser/include")
include_directories("${CMAKE_SOURCE_DIR}/log/include")

# files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.hpp")
add_custom_target(Headers SOURCES ${HEADERS})

if(NOT STD_FILESYSTEM)
    # I hope to use the upcoming standard filesystem library when it comes out, but until then use boost filesystem.
    find_package(Boost 1.57 COMPONENTS filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
else(NOT STD_FILESYSTEM)
    add_definitions(-DSTD_FILESYSTEM)
endif(NOT STD_FILESYSTEM)

# use to create ${PROJECT_NAME}_test executables.
macro(ADD_TEST_EXE)
    string(TOUPPER ${PROJECT_NAME} CAPSNAME)
    option(BUILD_${CAPSNAME}_TEST "Builds test binary for ${PROJECT_NAME}" OFF)
    if(BUILD_${CAPSNAME}_TEST)
        add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
        target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} log ${ARGN})
    endif()
endmacro()

add_subdirectory(log)
add_subdirectory(bigreader)
add_subdirectory(iniparser)

if(BUILD_SERVER)
    add_subdirectory(server)
endif(BUILD_SERVER)

if(BUILD_CLIENT)
    add_subdirectory(client)
endif(BUILD_CLIENT)
