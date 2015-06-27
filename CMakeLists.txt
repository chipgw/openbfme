project(openbfme)
cmake_minimum_required(VERSION 3.1)

# Put all binaries into build/bin folder.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

# MSVC uses WIP std filesystem library, anything else uses boost.
set(FS_LIBRARY)
if(NOT MSVC)
    find_package(Boost 1.57 COMPONENTS system filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(FS_LIBRARY ${Boost_LIBRARIES})

    list(APPEND CMAKE_CXX_FLAGS "-fvisibility=hidden")
endif()

# Enable any C++11 compiler flags if possible/nessecary.
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include_directories("${CMAKE_SOURCE_DIR}/include") # generic header-only includes.

# Where the add_project_lib() and add_project_exe() macros are defined.
include(OpenBFMEMacros)

add_custom_target(test_files SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.big)
add_custom_command(TARGET test_files COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/test.big
                                                                      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/test.big)
add_custom_command(TARGET test_files COMMAND big_extractor -o -d -s test.big WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

option(BUILD_ALL "Enables building all binaries" OFF)
option(BUILD_ALL_TESTS "Enables building all test binaries" OFF)

add_project_lib(core ${FS_LIBRARY})
add_project_lib(bigreader core ${FS_LIBRARY})
add_project_lib(iniparser bigreader)
add_project_lib(engine iniparser)

add_project_exe(big_extractor bigreader)
add_project_exe(make_big bigreader)
add_project_exe(openbfme_server iniparser)
add_project_exe(openbfme_client iniparser)
