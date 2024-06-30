# The Conway's Game of Fife in C++ with WxWidgets

This is a desktop program capable of running the Conway's Game of Life. It 
stores the game's initial positions in JSON files. When opened it opens a file
and runs the game, showing it in a grid in the GUI window, with also some info
like the generation number and the 

These features are planned to be added in the future:
- Runtime settings configuration
- Position selection
- Position creation
- Better GUI
- Better README and documentation

# Requirements/dependencies
- [CMake](https://cmake.org/) as build system
- [wxWidgets](https://www.wxwidgets.org) for GUI
- [nlohmann_json](https://github.com/nlohmann/json) for serialization and
  deserialization of JSON files which contains the position information
- [Catch2](https://github.com/catchorg/Catch2) as testing framework
- [Python](https://www.python.org/) for the temporal positions generation 
  scripts. not strictly necessary though