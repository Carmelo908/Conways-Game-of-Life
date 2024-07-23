# The Conway's Game of Fife in C++ with WxWidgets

This is a desktop program capable of running the Conway's Game of Life. It 
stores the game's initial positions in JSON files. when runned it opens a JSON
file and lets the user start the game. While working, it shows the positions in
a grid in the GUI window. It also shows information like the generation number 
and the amount of live cells.

These features are planned to be added in the future:
- Settings saving in a TOML file
- Position creation
- Position serialization and the removal of nlohmann_json from the project
- Better GUI
- Better README and documentation

# Requirements/dependencies
- [vcpkg](https://vcpkg.io/) as dependency manager
- [CMake](https://cmake.org/) as build system
- [wxWidgets](https://www.wxwidgets.org) as GUI framework
- [nlohmann_json](https://github.com/nlohmann/json) for serialization and
  deserialization of JSON files which contains the positions information
- [Catch2](https://github.com/catchorg/Catch2) as testing framework
- [Python](https://www.python.org/) for the temporal positions generation 
  scripts. not strictly necessary though