cmake_minimum_required(VERSION 3.1)

project(pharser CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE "Release")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_subdirectory("${PROJECT_SOURCE_DIR}/test")

add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
