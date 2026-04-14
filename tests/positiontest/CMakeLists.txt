set(
  SOURCE_FILES
  positiontest.cpp 
  ${PROJECT_SOURCE_DIR}/src/position.cpp
)

set(CURRENT_TEST position)

add_executable(${CURRENT_TEST} ${SOURCE_FILES})

target_include_directories(${CURRENT_TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${CURRENT_TEST} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_test(${CURRENT_TEST} ${CURRENT_TEST})