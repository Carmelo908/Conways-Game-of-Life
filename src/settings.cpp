#include "settings.hpp"

#include <filesystem>
#include <fstream>

#include <nlohmann/detail/exceptions.hpp>
#include <nlohmann/json.hpp>

bool validatePath(const std::filesystem::path &p)
{
  return std::filesystem::exists(p);
}

bool validateDelay(const std::chrono::milliseconds &d)
{
  return d.count() >= 10 && d.count() <= 1000;
}

Settings parseFileSettings(const std::filesystem::path &settingsFilePath)
{
  std::ifstream f{settingsFilePath};
  try
  {
    const auto parsingData = nlohmann::json::parse(f);
    return parseSettings(parsingData);
  } catch (const nlohmann::detail::exception &)
  {
    return Settings();
  }
}

Settings parseSettings(const nlohmann::json &settingsJson)
{
  Settings settings{};
  if (settingsJson.contains("position_path") &&
      settingsJson.at("position_path").is_string())
  {
    settings.setPositionPath(
        settingsJson.at("position_path").get<std::string>());
  }
  if (settingsJson.contains("delay") && settingsJson.at("delay").is_number())
  {
    auto delayms = settingsJson.at("delay").get<int>();
    settings.setDelay(std::chrono::milliseconds(delayms));
  }
  return settings;
}

nlohmann::json settingsToJson(const Settings &settings)
{
  nlohmann::json j{};
  j["position_path"] = settings.getPositionPath().string();
  j["delay"] = settings.getDelay().count();
  return j;
}

void saveSettings(const Settings &settings,
                  const std::filesystem::path &settingsPath)
{
  std::ofstream jsonFile{settingsPath};
  jsonFile << settingsToJson(settings).dump(2);
}

Settings::Settings()
  : positionPath{},
    delay{50}
{}

Settings::Settings(const std::filesystem::path &positionPath,
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