#pragma once

#include <chrono>
#include <filesystem>

#include <nlohmann/json_fwd.hpp>

/// @brief represents a user Settings container.
///
/// It can be loaded and saved in JSON format and its values are used in the
/// program runtime. Its fields are:
/// positionPath = a file path to the last opened position file.
/// delay = a duration in milliseconds for which the UI should pause between
/// every generation.
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
  Settings(const std::filesystem::path &positionPath,
           std::chrono::milliseconds delay, int zoom);

  /// @brief default comparision operator.
  bool operator==(const Settings &other) const = default;

  /// @return the positionPath field.
  const std::filesystem::path &getPositionPath() const;

  /// @return the delay field.
  std::chrono::milliseconds getDelay() const;

  int getZoom() const;

  /// @param d only sets the value if it's a existing file.
  void setPositionPath(const std::filesystem::path &p);

  /// @param p only sets the value if d.count() is in range (0, 1000).
  void setDelay(std::chrono::milliseconds d);

  void setZoom(int z);

private:
  std::filesystem::path positionPath;
  std::chrono::milliseconds delay;
  int zoom;
};

/// \related Settings
/// @brief Converts a Settings object to a nlohmann::json object.
nlohmann::json settingsToJson(const Settings &settings);

/// \related Settings
/// @param settingsFilePath a file which is expected to exist.
/// @return a Settings object which can have some or all of its fields set to
/// their default values if a valid value couldn't be found in the settings
/// file. If the input file is empty, it will return a default constructed
/// Settings object.
Settings parseFileSettings(const std::filesystem::path &settingsFilePath);

/// \related Settings
/// @brief converts a nlohmann::json object to a Settings object.
///
/// It tries to find values in the JSON object and in case it
/// doesn't find a field the Settings will set the default value for the field
/// instead.
Settings parseSettings(const nlohmann::json &settingsJson);

/// \related Settings
/// @param settings the Settings to save.
/// @param settingsPath The file's path where settings is saved.
void saveSettings(
    const Settings &settings,
    const std::filesystem::path &settingsPath = "./settings.json");