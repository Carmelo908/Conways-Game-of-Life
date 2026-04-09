#include <chrono>
#include <filesystem>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include "position.hpp"

#pragma once

using namespace std::chrono_literals;

constexpr char defaultSavePath[] = "./settings.json";

class SettingsData;

std::string settingsToJson(const SettingsData &settings);

SettingsData parseFileSettings(std::filesystem::path settingsFilePath);

SettingsData parseSettings(const nlohmann::json &settingsJson);

void saveSettings(const SettingsData &settings,
                  std::filesystem::path settingsPath);

class SettingsData
{
public:
  SettingsData();

  SettingsData(std::string_view posFilePath,
               std::chrono::milliseconds delay = 50ms);

  bool operator==(const SettingsData &other) const = default;

  std::string positionPath;
  std::chrono::milliseconds delay;
};