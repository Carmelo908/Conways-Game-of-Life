#include <chrono>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "position.hpp"

#pragma once

class SettingsData
{
public:
  SettingsData();

  SettingsData(std::filesystem::path posFilePath,
               std::chrono::milliseconds delay);

  bool operator==(const SettingsData &other) const = default;

  std::filesystem::path positionPath;
  std::chrono::milliseconds delay;
};

nlohmann::json settingsToJson(const SettingsData &settings);

SettingsData parseFileSettings(std::filesystem::path settingsFilePath);

SettingsData parseSettings(const nlohmann::json &settingsJson);

void saveSettings(const SettingsData &settings,
                  std::filesystem::path settingsPath);