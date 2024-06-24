cmake_minimum_required(VERSION 3.22.1)
project(processing)

add_executable(${PROJECT_NAME} processing.cpp)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ../../src/
)