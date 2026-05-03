#include "settings.hpp"

#include <fstream>

bool validatePath(const std::filesystem::path &p)
{
  return std::filesystem::exists(p);
}

bool validateDelay(std::chrono::milliseconds d)
{
  return d.count() > 0 && d.count() < 1000;
}

Settings parseFileSettings(std::filesystem::path settingsFilePath)
{
  std::ifstream f{settingsFilePath};
  const auto parsingData = nlohmann::json::parse(f);
  return parseSettings(parsingData);
}

Settings parseSettings(const nlohmann::json &settingsJson)
{
  Settings settings{};
  if (settingsJson.contains("position_path") &&
      settingsJson["position_path"].is_string())
  {
    settings.setPositionPath(settingsJson["position_path"].get<std::string>());
  }
  if (settingsJson.contains("delay") && settingsJson["delay"].is_number())
  {
    auto delayms = settingsJson["delay"].get<int>();
    settings.setDelay(std::chrono::milliseconds(delayms));
  }
  return settings;
}

nlohmann::json settingsToJson(const Settings &settings)
{
  nlohmann::json j;
  j["position_path"] = settings.getPositionPath().string();
  j["delay"] = settings.getDelay().count();
  return j;
}

void saveSettings(const Settings &settings, std::filesystem::path settingsPath)
{
  std::ofstream jsonFile{settingsPath};
  jsonFile << settingsToJson(settings).dump(2);
}

Settings::Settings()
  : positionPath{},
    delay{50}
{}

Settings::Settings(std::filesystem::path positionPath,
                   std::chrono::milliseconds delay)
  : Settings()
{
  if (validatePath(positionPath))
  {
    this->positionPath = positionPath;
  }
  if (validateDelay(delay))
  {
    this->delay = delay;
  }
}

const std::filesystem::path &Settings::getPositionPath() const
{
  return positionPath;
}

std::chrono::milliseconds Settings::getDelay() const { return delay; }

void Settings::setPositionPath(const std::filesystem::path &p)
{
  if (validatePath(p))
  {
    positionPath = p;
  }
}

void Settings::setDelay(std::chrono::milliseconds d)
{
  if (validateDelay(d))
  {
    delay = d;
  }
}
