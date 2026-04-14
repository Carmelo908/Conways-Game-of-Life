set(LIBRARY_NAME gui)

set(
  SOURCE_FILES
  configmenu.cpp
  fieldspanel.cpp
  gameframe.cpp
  drawingpanel.cpp
)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PUBLIC nlohmann_json::nlohmann_json)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PUBLIC wx::core wx::base)