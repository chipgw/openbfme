project(openbfme)
cmake_minimum_required(VERSION 3.1)

# Put all binaries into build/bin folder.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

# MSVC uses WIP std filesystem library, anything else uses boost.
set(FS_LIBRARY)
if(NOT MSVC)
    find_package(Boost 1.57 COMPONENTS filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(FS_LIBRARY ${Boost_FILESYSTEM_LIBRARY_RELEASE})
endif()

# Enable any C++11 compiler flags if possible/nessecary.
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

find_package(TinyXML REQUIRED)
include_directories(${TINYXML_INCLUDE_DIR})

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.
include_directories("${CMAKE_SOURCE_DIR}/log/include")
include_directories("${CMAKE_SOURCE_DIR}/bigreader/include")
include_directories("${CMAKE_SOURCE_DIR}/iniparser/include")

# Files not needed in build, but included so they show up in IDE.
file(GLOB_RECURSE HEADERS "*.hpp")
add_custom_target(IDE_FILES SOURCES ${HEADERS})

add_custom_target(test_big)
add_custom_command(TARGET test_big COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/test.big
                                                                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/test.big)
add_custom_command(TARGET test_big COMMAND big_extractor test.big WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

option(BUILD_ALL "Enables building all binaries" OFF)
option(BUILD_ALL_TESTS "Enables building all test binaries" OFF)

# Where the add_project_lib() and add_project_exe() macros are defined.
include(OpenBFMEMacros)

add_project_lib(log)
add_project_lib(bigreader log ${FS_LIBRARY})
add_project_lib(iniparser bigreader ${TINYXML_LIBRARIES})

add_project_exe(big_extractor bigreader)
add_project_exe(openbfme_server iniparser)
add_project_exe(openbfme_client iniparser)
