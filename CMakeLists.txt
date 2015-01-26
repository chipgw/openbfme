project(openbfme)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    find_package(Boost 1.57 COMPONENTS filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(FS_LIBRARY ${Boost_FILESYSTEM_LIBRARY_RELEASE})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(FS_LIBRARY)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

find_package(TinyXML REQUIRED)
include_directories(${TINYXML_INCLUDE_DIR})

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.
include_directories("${CMAKE_SOURCE_DIR}/bigreader/include")
include_directories("${CMAKE_SOURCE_DIR}/iniparser/include")
include_directories("${CMAKE_SOURCE_DIR}/log/include")

# files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.hpp")
add_custom_target(IDE_FILES SOURCES ${HEADERS})

option(BUILD_ALL "Enables building all binaries" OFF)
option(BUILD_ALL_TESTS "Enables building all test binaries" OFF)

include(OpenBFMEMacros)

add_project_lib(log)
add_project_lib(bigreader log ${FS_LIBRARY})
add_project_lib(iniparser bigreader ${TINYXML_LIBRARIES})

add_project_exe(big_extractor bigreader)
add_project_exe(openbfme_server iniparser)
add_project_exe(openbfme_client iniparser)
