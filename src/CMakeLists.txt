set(
  SOURCE_FILES
  main.cpp
  position.cpp
  settings.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(gui)

target_link_libraries(${PROJECT_NAME} PRIVATE gui)
target_include_directories(${PROJECT_NAME} PRIVATE gui)