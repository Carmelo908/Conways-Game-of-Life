cmake_minimum_required(VERSION 3.29.6)
project(TESTS)

set(
  UNIT_TESTS 
  processing
)

foreach(TEST_NAME IN LISTS UNIT_TESTS)
    add_subdirectory(${TEST_NAME})
endforeach()

message("Configurated tests:")
foreach(TEST_NAME IN LISTS UNIT_TESTS)
  message("- ${TEST_NAME}")
endforeach()