#include <filesystem>
#include <string>
#include <string_view>

#include "settingsdata.hpp"

#include <nlohmann/json.hpp>

#pragma once

constexpr char defaultSavePath[] = "./settings.json";

std::string settingsToJson(SettingsData settings);

SettingsData parseFileSettings(std::filesystem::path settingsFilePath);

SettingsData parseSettings(nlohmann::json &settingsJson);

void saveSettings(SettingsData settings,
                  std::string_view settingsFilePath = defaultSavePath);