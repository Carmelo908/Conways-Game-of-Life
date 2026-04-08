cmake_minimum_required(VERSION 4.0.0)
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

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)