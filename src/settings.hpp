#pragma once

#include <chrono>
#include <filesystem>

#include <nlohmann/json_fwd.hpp>

class Settings
{
public:
  /// @brief Contructs Settings with default values.
  ///
  /// The default values are delay.count() = 50 and a empty file path.
  Settings();

  /// @brief Constructs Settings with default values and only saves the
  /// arguments if they accomplish specific conditions.
  /// @param posFilePath must be an existing JSON file.
  /// @param delay count() must be in range (0, 1000).
  Settings(std::filesystem::path posFilePath, std::chrono::milliseconds delay);

  bool operator==(const Settings &other) const = default;

  const std::filesystem::path &getPositionPath() const;
  std::chrono::milliseconds getDelay() const;

  /// @param d only sets the value if it's a existing file.
  void setPositionPath(const std::filesystem::path &p);

  /// @param p only sets the value if d.count() is in range (0, 1000).
  void setDelay(std::chrono::milliseconds d);

private:
  std::filesystem::path positionPath;
  std::chrono::milliseconds delay;
};

/// @brief Converts a Settings object to a nlohmann::json object.
nlohmann::json settingsToJson(const Settings &settings);

/// @param settingsFilePath a file which is expected to exist.
/// @return a Settings object which can have some or all of its fields set to
/// their default values if a valid value couldn't be found in the settings
/// file.
Settings parseFileSettings(std::filesystem::path settingsFilePath);

/// @brief converts a nlohmann::json object to a Settings object.
///
/// It tries to find values in the JSON object and in case it
/// doesn't find a field the Settings will the default value for the field
/// instead.
Settings parseSettings(const nlohmann::json &settingsJson);

/// @param settings the saved Settings
/// @param settingsPath The file's path where settings is saved
void saveSettings(const Settings &settings, std::filesystem::path settingsPath);