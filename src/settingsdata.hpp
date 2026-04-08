#include <chrono>
#include <memory>
#include <string>
#include <variant>

#include "position.hpp"

#pragma once

using namespace std::chrono_literals;

class SettingsData
{
public:
  SettingsData()
    : SettingsData("", 50ms)
  {}

  SettingsData(std::string_view posFilePath,
               std::chrono::milliseconds delay = 50ms);

  bool operator==(const SettingsData &other) const = default;

  std::string positionPath;
  std::chrono::milliseconds delay;
};