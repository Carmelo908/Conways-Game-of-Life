#include "settings.hpp"

#include <format>
#include <fstream>

Settings parseFileSettings(std::filesystem::path settingsFilePath)
{
  std::ifstream f{settingsFilePath};
  const auto parsingData = nlohmann::json::parse(f);
  return parseSettings(parsingData);
}

Settings parseSettings(const nlohmann::json &settingsJson)
{
  std::string positionPath{settingsJson["position_path"].get<std::string>()};
  std::chrono::milliseconds delay{settingsJson["delay"].get<int>()};
  return Settings(positionPath, delay);
}

nlohmann::json settingsToJson(const Settings &settings)
{
  nlohmann::json j;
  j["position_path"] = settings.positionPath.string();
  j["delay"] = settings.delay.count();
  return j;
}

void saveSettings(const Settings &settings, std::filesystem::path settingsPath)
{
  std::ofstream jsonFile{settingsPath};
  jsonFile << settingsToJson(settings);
}

Settings::Settings()
  : Settings("", std::chrono::milliseconds(50))
{}

Settings::Settings(std::filesystem::path posFilePath,
                   std::chrono::milliseconds delay)
  : delay{delay}
{
  positionPath = posFilePath;
}