cmake_minimum_required(VERSION 3.22.1)
project(TESTS)

set(UNIT_TESTS processing)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

foreach(TEST_NAME IN LISTS UNIT_TESTS)
    add_executable(${TEST_NAME} ./${TEST_NAME}/${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} PRIVATE wx::core wx::base)
    target_link_libraries(${TEST_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${TEST_NAME} PRIVATE 
    Catch2::Catch2 Catch2::Catch2WithMain)
    target_include_directories(
        ${TEST_NAME}
        PUBLIC ../src/
        PUBLIC ../src/gui/
    )
    set_target_properties(${TEST_NAME} 
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./${TEST_NAME})
endforeach()

message("Configurated tests:")
foreach(TEST_NAME IN LISTS UNIT_TESTS)
  message("${TEST_NAME}")
endforeach()