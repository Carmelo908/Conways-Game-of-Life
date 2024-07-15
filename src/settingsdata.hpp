#include <string>
#include <chrono>
#include <memory>
#include <variant>

#include "position.hpp"

#pragma once

using namespace std::chrono_literals;

class SettingsData
{
public:
  SettingsData() : SettingsData("", 0ms) {}

  SettingsData(std::string_view posFilePath,
  std::chrono::milliseconds delay = 0ms) :
    delay {delay}
  {
    position = posFilePath.data();
  };

  std::string_view getUnopenedPath()
  {
    return std::string_view(std::get<std::string>(position));
  }

  std::unique_ptr<Position> releaseOpenedPosition()
  {
    auto posPointer = std::get<position_ptr>(position).release();
    return position_ptr(posPointer);
  }

  std::variant<std::string, std::unique_ptr<Position>> position;
  std::chrono::milliseconds delay;

private:
  using position_ptr = std::unique_ptr<Position>;
};
