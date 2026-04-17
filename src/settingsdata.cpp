#include "settingsdata.hpp"

#include <format>
#include <fstream>

SettingsData parseFileSettings(std::filesystem::path settingsFilePath)
{
  std::ifstream f{settingsFilePath};
  const auto parsingData = nlohmann::json::parse(f);
  return parseSettings(parsingData);
}

SettingsData parseSettings(const nlohmann::json &settingsJson)
{
  std::string positionPath{settingsJson["position_path"].get<std::string>()};
  std::chrono::milliseconds delay{settingsJson["delay"].get<int>()};
  return SettingsData(positionPath, delay);
}

nlohmann::json settingsToJson(const SettingsData &settings)
{
  nlohmann::json j;
  j["position_path"] = settings.positionPath.string();
  j["delay"] = settings.delay.count();
  return j;
}

void saveSettings(const SettingsData &settings,
                  std::filesystem::path settingsPath)
{
  std::ofstream jsonFile{settingsPath};
  jsonFile << settingsToJson(settings);
}

SettingsData::SettingsData()
  : SettingsData("", std::chrono::milliseconds(50))
{}

SettingsData::SettingsData(std::filesystem::path posFilePath,
                           std::chrono::milliseconds delay)
  : delay{delay}
{
  positionPath = posFilePath;
}