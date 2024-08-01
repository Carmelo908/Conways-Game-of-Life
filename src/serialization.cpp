#include <format>
#include <fstream>

#include <toml++/toml.hpp>

#include "serialization.hpp"

SettingsData parseFileSettings(std::string_view settingsFilePath)
{
  auto parsingData = toml::parse_file(settingsFilePath);
  return parseSettings(parsingData);
}

std::chrono::milliseconds getDelay(toml::table &parsingData)
{
  int delay = parsingData["settings"]["delay"].value_or<int>(50);
  return std::chrono::milliseconds(delay);
}

std::string getPositionPath(toml::table &parsingData)
{
  return parsingData["settings"]["position_path"].value_or<std::string>("");
}

SettingsData parseSettings(toml::table &parsingData)
{
  std::chrono::milliseconds delay = getDelay(parsingData);
  std::string positionPath = getPositionPath(parsingData);
  return SettingsData(positionPath, delay);
}

SettingsData parseSettings(std::string_view tomlString)
{
  auto parsingData = toml::parse(tomlString);
  return parseSettings(parsingData);
}

std::string settingsToToml(SettingsData settings)
{
  return std::format("[settings]\ndelay = {}\nposition_path = \"{}\"",
                     settings.delay.count(), settings.positionPath);
}

void saveSettings(SettingsData settings, std::string_view settingsFilePath)
{
  std::ofstream tomlFile{settingsFilePath.data()};
  tomlFile << settingsToToml(settings);
}