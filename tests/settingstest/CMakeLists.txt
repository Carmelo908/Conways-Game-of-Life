set(
  SOURCE_FILES
  settingstest.cpp 
  ${PROJECT_SOURCE_DIR}/src/settings.cpp
)

set(CURRENT_TEST settings)

add_executable(${CURRENT_TEST} ${SOURCE_FILES})

target_include_directories(${CURRENT_TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src)





add_test(${CURRENT_TEST} ${CURRENT_TEST})