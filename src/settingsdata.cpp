#include "settingsdata.hpp"

#include <format>
#include <fstream>

SettingsData parseFileSettings(std::filesystem::path settingsFilePath)
{
  std::ifstream f{settingsFilePath};
  auto parsingData = nlohmann::json::parse(f);
  return parseSettings(parsingData);
}

SettingsData parseSettings(nlohmann::json &settingsJson)
{
  std::string positionPath{settingsJson["position_path"].get<std::string>()};
  std::chrono::milliseconds delay{settingsJson["delay"].get<int>()};
  return SettingsData(positionPath, delay);
}

std::string settingsToJson(SettingsData settings)
{
  return std::format("{{\"delay\": {}, \"position_path\": \"{}\"}}",
                     settings.delay.count(), settings.positionPath);
}

void saveSettings(SettingsData settings, std::string_view settingsFilePath)
{
  std::ofstream jsonFile{settingsFilePath.data()};
  jsonFile << settingsToJson(settings);
}

SettingsData::SettingsData()
  : SettingsData("", 50ms)
{}

SettingsData::SettingsData(std::string_view posFilePath,
                           std::chrono::milliseconds delay)
  : delay{delay}
{
  positionPath = posFilePath.data();
}