set(
  SOURCE_FILES
  main.cpp
  position.cpp
  settings.cpp
  gui/settingspanel.cpp
  gui/gameframe.cpp
  gui/positionpanel.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC wx::core wx::base)