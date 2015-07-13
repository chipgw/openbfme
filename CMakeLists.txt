set(CMAKE_BUILD_TYPE_INIT "Debug")

project(openbfme)
cmake_minimum_required(VERSION 3.2)

# Put all binaries into build/bin folder.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for static libraries.")

# If this is blank we use the WIP std filesystem library.
set(FS_LIBRARY)

# GCC and Clang reqiure Boost for the filesystem library until they get std implementations. */
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_package(Boost 1.57 COMPONENTS system filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(FS_LIBRARY ${Boost_LIBRARIES})

    # GCC and Clang also need a flag to only export functions marked as EXPORT.
    list(APPEND CMAKE_CXX_FLAGS "-fvisibility=hidden")
endif()

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# Enable any C++11 compiler flags if possible/nessecary.
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Where the add_project_lib() and add_project_exe() macros are defined.
include(OpenBFMEMacros)

# Set up the commands to put the test files in the bin directory.
# All test exe targets depend on this target, so it's only run when one of them is enabled.
add_custom_target(test_files SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.big)
add_custom_command(TARGET test_files COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/test.big
                                                                      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/test.big)
add_custom_command(TARGET test_files COMMAND big_extractor -o -d -s test.big WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

option(BUILD_ALL "Enables building all binaries" OFF)
option(BUILD_ALL_TESTS "Enables building all test binaries" OFF)

# First arg is lib name, any following are libs that the target depends on.
add_project_lib(core ${FS_LIBRARY})
add_project_lib(bigreader core ${FS_LIBRARY})
add_project_lib(iniparser bigreader)
add_project_lib(engine iniparser)

# First arg is exe name, any following are libs that the target depends on.
add_project_exe(big_extractor bigreader)
add_project_exe(make_big bigreader)
add_project_exe(openbfme_server iniparser)
add_project_exe(openbfme_client iniparser)
