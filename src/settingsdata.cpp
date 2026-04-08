#include "settingsdata.hpp"

SettingsData::SettingsData(std::string_view posFilePath,
                           std::chrono::milliseconds delay)
  : delay{delay}
{
  positionPath = posFilePath.data();
}