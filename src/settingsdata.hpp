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
               std::chrono::milliseconds delay = 50ms)
    : delay{delay}
  {
    positionPath = posFilePath.data();
  }

  bool operator==(const SettingsData &other) const
  {
    return this->delay == other.delay &&
           this->positionPath == other.positionPath;
  }

  std::string positionPath;
  std::chrono::milliseconds delay;
};