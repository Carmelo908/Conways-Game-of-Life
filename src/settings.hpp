#include <chrono>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "position.hpp"

#pragma once

class Settings
{
public:
  Settings();

  Settings(std::filesystem::path posFilePath, std::chrono::milliseconds delay);

  bool operator==(const Settings &other) const = default;

  std::filesystem::path positionPath;
  std::chrono::milliseconds delay;
};

nlohmann::json settingsToJson(const Settings &settings);

Settings parseFileSettings(std::filesystem::path settingsFilePath);

Settings parseSettings(const nlohmann::json &settingsJson);

void saveSettings(const Settings &settings, std::filesystem::path settingsPath);