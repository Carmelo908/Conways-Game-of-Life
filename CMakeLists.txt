cmake_minimum_required(VERSION 4.0.0)
project(Conway)

option(Conway_ENABLE_TESTING "Enable testing" ${PROJECT_IS_TOP_LEVEL})
set(CMAKE_CXX_STANDARD 23)

if(Conway_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(src)