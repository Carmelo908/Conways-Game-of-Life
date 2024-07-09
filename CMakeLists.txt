cmake_minimum_required(VERSION 3.29.6)
project(Conway)

add_executable(${PROJECT_NAME} src/main.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)

add_compile_options("-Wall -Wextra")

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE src/
  PRIVATE src/gui
  PRIVATE src/tomlconfig
)