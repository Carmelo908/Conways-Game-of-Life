cmake_minimum_required(VERSION 3.29.6)

set(
  UNIT_TESTS 
  positiontest
  settingstest
)

set(TESTS_ARGS --skip-benchmarks)

find_package(Catch2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(
  positiontest 
  positiontest.cpp
  ${CMAKE_SOURCE_DIR}/src/position.cpp
)

add_executable(
  settingstest
  settingstest.cpp
  ${CMAKE_SOURCE_DIR}/src/settings.cpp
)
target_link_libraries(settingstest PRIVATE nlohmann_json::nlohmann_json)

foreach(TEST_NAME IN LISTS UNIT_TESTS)
  add_test(${TEST_NAME} ${TEST_NAME} ${TESTS_ARGS})
  target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)  
  target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
endforeach()