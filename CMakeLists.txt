cmake_minimum_required(VERSION 3.22.1)
project(Conway)

add_executable(${PROJECT_NAME} src/main.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -g")

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(nlohmann_json 3.10.5 REQUIRED)

include(${wxWidgets_USE_FILE})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC src/
)