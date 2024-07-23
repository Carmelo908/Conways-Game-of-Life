cmake_minimum_required(VERSION 3.29.6)
project(processing)

include(${CMAKE_SOURCE_DIR}/Settings.cmake)

set(
  SOURCE_FILES
  processing.cpp 
  ${PROJECT_SOURCE_DIR}/position.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_compile_options(${TESTS_COMPILATIONS_SETTINGS})

find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)