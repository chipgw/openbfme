# Use to create test executables.
macro(ADD_TEST_EXE)
    string(TOUPPER "BUILD_${ARGV0}_TEST" OPTION_NAME)

    if(BUILD_ALL_TESTS)
        unset(${OPTION_NAME} CACHE)
    else()
        option(${OPTION_NAME} "Builds test binary for ${ARGV0}" OFF)
    endif()

    if(${OPTION_NAME} OR BUILD_ALL_TESTS)
        add_executable(${ARGV0}_test test/${ARGV0}_test.cpp)
        target_link_libraries(${ARGV0}_test ${ARGN})
    endif()
endmacro()

# Use to create libraries.
macro(ADD_PROJECT_LIB)
    file(GLOB SOURCES "${ARGV0}/*.cpp")

    add_library(${ARGV0} SHARED ${SOURCES})
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
        target_link_libraries(${ARGN})
    endif()
endmacro()
