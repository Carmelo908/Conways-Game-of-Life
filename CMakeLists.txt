cmake_minimum_required(VERSION 3.29.6)
project(Conway)

set(
  SOURCE_FILES
  src/main.cpp
  src/position.cpp
  src/serialization.cpp
  src/gui/configmenu.cpp
  src/gui/fieldspanel.cpp
  src/gui/gameframe.cpp
)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_compile_options("-Wall -Wextra")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::tomlplusplus)