cmake_minimum_required(VERSION 2.8)
project(log)

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

add_test_exe()
