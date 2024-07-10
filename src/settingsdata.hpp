#include <string>
#include <chrono>

#pragma once

using namespace std::chrono_literals;

class SettingsData
{
public:
  SettingsData() : SettingsData("", 0ms) {}

  SettingsData(std::string_view posPath,
  std::chrono::milliseconds delay = 0ms) :
    posPath {posPath},
    delay {delay}
  {}

  std::string posPath;
  std::chrono::milliseconds delay;
};
