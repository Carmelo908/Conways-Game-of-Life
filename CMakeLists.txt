cmake_minimum_required(VERSION 3.29.6)
project(Conway)

set(
  SOURCE_FILES 
  src/main.cpp 
  src/position.cpp
  src/gui/configmenu.cpp
  src/gui/fieldspanel.cpp
  src/gui/gameframe.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)

add_compile_options("-Wall -Wextra")