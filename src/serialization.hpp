#include <string>
#include <string_view>

#include "settingsdata.hpp"

#include <toml++/toml.hpp>

#pragma once

constexpr char defaultSavePath[] = "./settings.toml";

SettingsData
parseFileSettings(std::string_view settingsFilePath);

SettingsData parseSettings(toml::table &parsingData);
SettingsData parseSettings(std::string_view tomlString);

std::string settingsToToml(SettingsData settings);

void saveSettings(SettingsData settings,
                  std::string_view settingsFilePath = defaultSavePath);