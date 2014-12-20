cmake_minimum_required(VERSION 2.8)
project(openbfme_server)

include_directories("./include")

# files not needed in build, but included so they show up in IDE.
file(GLOB HEADERS "*.h")

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} log)
