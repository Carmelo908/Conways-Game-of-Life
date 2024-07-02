cmake_minimum_required(VERSION 3.22.1)

set(UNIT_TESTS processing)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -g")

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(nlohmann_json 3.10.5 REQUIRED)
include(${wxWidgets_USE_FILE})

foreach(TEST_NAME IN LISTS UNIT_TESTS)
  add_executable(${TEST_NAME} ./${TEST_NAME}/${TEST_NAME}.cpp)
  target_link_libraries(${TEST_NAME} ${wxWidgets_LIBRARIES})
  target_include_directories(
    ${TEST_NAME}
    PUBLIC ../src/
    PUBLIC ../src/tomlconfig/
    PUBLIC ../src/gui/
  )
  set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./${TEST_NAME})

endforeach()

message("Configurated projects: " ${UNIT_TESTS})