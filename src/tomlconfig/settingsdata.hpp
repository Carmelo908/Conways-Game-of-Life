#include <string>
#include <chrono>

#pragma once

namespace chrono = std::chrono;

class SettingsData
{
public:
  SettingsData(std::string_view posPath, 
  chrono::milliseconds delay = chrono::milliseconds(0)) :
    posPath {posPath},
    delay {delay}
  {}

  SettingsData() : posPath {""} {}

  std::string posPath;
  chrono::milliseconds delay;
};
