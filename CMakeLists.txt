project(openbfme)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

option(USE_STD_FILESYSTEM "Use system provided filesystem library instead of boost filesystem. Only tested with MSVC currently." OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.
include_directories("${CMAKE_SOURCE_DIR}/bigreader/include")
include_directories("${CMAKE_SOURCE_DIR}/iniparser/include")
include_directories("${CMAKE_SOURCE_DIR}/log/include")

# files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.hpp")
add_custom_target(IDE_FILES SOURCES ${HEADERS})

if(USE_STD_FILESYSTEM)
    add_definitions(-DSTD_FILESYSTEM)
    set(FS_LIBRARY)
else(USE_STD_FILESYSTEM)
    find_package(Boost 1.57 COMPONENTS filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(FS_LIBRARY ${Boost_FILESYSTEM_LIBRARY_RELEASE})
endif(USE_STD_FILESYSTEM)

option(BUILD_ALL "Enables building all binaries" OFF)
option(BUILD_ALL_TESTS "Enables building all test binaries" OFF)

# Use to create ${PROJECT_NAME}_test executables.
macro(ADD_TEST_EXE)
    string(TOUPPER "BUILD_${ARGV0}_TEST" OPTION_NAME)

    if(BUILD_ALL OR BUILD_ALL_TESTS)
        unset(${OPTION_NAME} CACHE)
    else()
        option(${OPTION_NAME} "Builds test binary for ${ARGV0}" OFF)
    endif()

    if(${OPTION_NAME} OR BUILD_ALL_TESTS)
        add_executable(${ARGV0}_test test/${ARGV0}_test.cpp)
        target_link_libraries(${ARGV0}_test ${ARGN} log)
    endif()
endmacro()

# Use to create libraries.
macro(ADD_PROJECT_LIB)
    file(GLOB SOURCES "${ARGV0}/*.cpp")

    add_library(${ARGV0} SHARED ${SOURCES})

    if(WIN32)
        target_link_libraries(${ARGV0} log)
    endif()

    target_link_libraries(${ARGN})

    add_test_exe(${ARGV0})
endmacro()

# Use to create executables.
# TODO - Add some way of setting the option description.
macro(ADD_PROJECT_EXE)
    string(TOUPPER "BUILD_${ARGV0}" OPTION_NAME)

    if(BUILD_ALL)
        unset(${OPTION_NAME} CACHE)
    else()
        option(${OPTION_NAME} "Builds ${ARGV0} binary" OFF)
    endif()

    if(${OPTION_NAME} OR BUILD_ALL)
        include_directories("./include")

        file(GLOB SOURCES "${ARGV0}/*.cpp")

        add_executable(${ARGV0} ${SOURCES})
        target_link_libraries(${ARGN} log)
    endif()
endmacro()


add_project_lib(log)
add_project_lib(bigreader ${FS_LIBRARY})
add_project_lib(iniparser bigreader)

add_project_exe(big_extractor bigreader)
add_project_exe(openbfme_server)
add_project_exe(openbfme_client)
