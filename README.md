# The Conway's Game of Fife in C++ with WxWidgets

This is a desktop program capable of running the Conway's Game of Life. It
stores the game's initial positions in JSON files. when runned it opens a JSON
file and lets the user start the game. While working, it shows the positions in
a grid in the GUI window. It also shows information like the generation number
and the amount of live cells.

These features are planned to be added in the future:

- Position creation and serialization
- Documentation

## Requirements/dependencies

- [vcpkg](https://vcpkg.io/) as dependency manager
- [CMake](https://cmake.org/) as build system
- [wxWidgets](https://www.wxwidgets.org) as GUI framework (managed by vcpkg)
- [nlohmann_json](https://github.com/nlohmann/json) for serialization and
  deserialization of JSON files which contains the positions information
  (managed by vcpkg)
- [Catch2](https://github.com/catchorg/Catch2) as testing framework
  (managed by vcpkg)
- [Python](https://www.python.org/) for the temporal positions generation
  scripts but not strictly necessary though

## How to compile

In order to compile the project, you need to have CMake and vcpkg installed.
Also, while the output buildsystem can be changed, Ninja is the default, so it
is needed to have it or another buildsystem installed. The first step is to
create the folder `<root>/build/<preset-name>`. The project has presets for the
compilers Clang, GCC and MSV; for both release and debug builds. Once in the
output directory, execute:

~~~ shell
vcpkg install
~~~

After all the project dependencies are installed, you can return to the project
root as the presets set the output directory. Configure with CMake:

~~~ shell
cmake . --preset <your-preset> -DCMAKE_TOOLCHAIN_FILE="<your-path-to>/vcpkg/scripts/buildsystems/vcpkg.cmake"
~~~

Note that the toolchain file can be also passed in with a user preset. After
The project dependencies are installed and it was configured, execute:

~~~ shell
cmake --build build/default --target all --
~~~
