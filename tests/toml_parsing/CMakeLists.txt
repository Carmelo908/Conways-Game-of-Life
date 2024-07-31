cmake_minimum_required(VERSION 3.29.6)
project(toml_parsing)

include(${CMAKE_SOURCE_DIR}/Settings.cmake)

set(
  SOURCE_FILES
  toml_parsing.cpp
  ${PROJECT_SOURCE_DIR}/serialization.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_compile_options(${TESTS_COMPILATIONS_SETTINGS})

find_package(Catch2 CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Catch2::Catch2 Catch2::Catch2WithMain
  PRIVATE PkgConfig::tomlplusplus
)