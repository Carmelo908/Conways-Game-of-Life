set(
  SOURCE_FILES
  settingstest.cpp 
  ${PROJECT_SOURCE_DIR}/src/settings.cpp
)

set(CURRENT_TEST settings)

add_executable(${CURRENT_TEST} ${SOURCE_FILES})

target_include_directories(${CURRENT_TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${CURRENT_TEST} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${CURRENT_TEST} PRIVATE nlohmann_json::nlohmann_json)

add_test(${CURRENT_TEST} ${CURRENT_TEST})